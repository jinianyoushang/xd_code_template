#include <cstdlib>
#include <pthread.h>
#include <cstdint>

struct thread_pool_task {
    void *arg;                              // 任务参数
    void *(*func)(void *);                   // 任务函数指针
    struct thread_pool_task *next;          // 下一个任务指针
};

struct thread_pool {
    uint32_t thread_count;                   // 线程数
    pthread_t *threads;                      // 线程数组
    struct thread_pool_task *task_list_head; // 任务队列头指针
    struct thread_pool_task *task_list_tail; // 任务队列尾指针
    pthread_mutex_t mutex;                   // 互斥锁
    pthread_cond_t cond;                     // 条件变量
    bool running;                            // 线程池是否正在运行
};

static void *thread_pool_worker(void *arg);

static void thread_pool_add_task(struct thread_pool *pool, void *arg, void *(*func)(void *));

static struct thread_pool_task *thread_pool_get_task(struct thread_pool *pool);

struct thread_pool *thread_pool_create(uint32_t thread_count) {
    struct thread_pool *pool = (struct thread_pool *) malloc(sizeof(struct thread_pool));
    if (pool == NULL) {
        return NULL;
    }
    pool->thread_count = thread_count;
    pool->threads = (pthread_t *) malloc(sizeof(pthread_t) * thread_count);
    if (pool->threads == NULL) {
        free(pool);
        return NULL;
    }
    pool->task_list_head = NULL;
    pool->task_list_tail = NULL;
    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->cond, NULL);
    pool->running = true;
    for (uint32_t i = 0; i < thread_count; ++i) {
        pthread_create(&pool->threads[i], NULL, thread_pool_worker, (void *) pool);
    }
    return pool;
}

void thread_pool_destroy(struct thread_pool *pool) {
    if (pool == NULL) {
        return;
    }
    pthread_mutex_lock(&pool->mutex);
    pool->running = false;
    pthread_cond_broadcast(&pool->cond);
    pthread_mutex_unlock(&pool->mutex);
    for (uint32_t i = 0; i < pool->thread_count; ++i) {
        pthread_join(pool->threads[i], NULL);
    }
    struct thread_pool_task *task = pool->task_list_head;
    while (task != NULL) {
        struct thread_pool_task *next = task->next;
        free(task);
        task = next;
    }
    free(pool->threads);
    pthread_mutex_destroy(&pool->mutex);
    pthread_cond_destroy(&pool->cond);
    free(pool);
}

void thread_pool_submit(struct thread_pool *pool, void *arg, void *(*func)(void *)) {
    thread_pool_add_task(pool, arg, func);
}

static void *thread_pool_worker(void *arg) {
    struct thread_pool *pool = (struct thread_pool *) arg;
    while (pool->running) {
        struct thread_pool_task *task = thread_pool_get_task(pool);
        if (task == NULL) {
            continue;
        }
        void *result = task->func(task->arg);
        free(task);
    }
    return NULL;
}

static struct thread_pool_task *thread_pool_get_task(struct thread_pool *pool) {
    pthread_mutex_lock(&pool->mutex);
    while (pool->task_list_head == NULL && pool->running) {
        pthread_cond_wait(&pool->cond, &pool->mutex);
    }
    struct thread_pool_task *task = NULL;
    if (pool->task_list_head != NULL) {
        task = pool->task_list_head;
        if (pool->task_list_head == pool->task_list_tail) {
            pool->task_list_head = NULL;
            pool->task_list_tail = NULL;
        } else {
            pool->task_list_head = task->next;
        }
    }
    pthread_mutex_unlock(&pool->mutex);
    return task;
}

static void thread_pool_add_task(struct thread_pool *pool, void *arg, void *(*func)(void *)) {
    struct thread_pool_task *task = (struct thread_pool_task *) malloc(sizeof(struct thread_pool_task));
    task->arg = arg;
    task->func = func;
    task->next = NULL;
    pthread_mutex_lock(&pool->mutex);
    if (pool->task_list_head == NULL) {
        pool->task_list_head = task;
        pool->task_list_tail = task;
    } else {
        pool->task_list_tail->next = task;
        pool->task_list_tail = task;
    }
    pthread_cond_signal(&pool->cond);
    pthread_mutex_unlock(&pool->mutex);
}