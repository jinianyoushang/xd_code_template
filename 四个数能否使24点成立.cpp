bool canGet24(std::vector<double>& nums) {
    if (nums.size() == 1) {
        return std::abs(nums[0] - 24) < 1e-6;  // 检查最终结果是否接近24
    }

    // 枚举所有可能的两个数的组合
    for (int i = 0; i < nums.size(); ++i) {
        for (int j = 0; j < nums.size(); ++j) {
            if (i == j) continue;

            std::vector<double> next_nums;
            for (int k = 0; k < nums.size(); ++k) {
                if (k != i && k != j) {
                    next_nums.push_back(nums[k]);
                }
            }

            // 四则运算操作
            double a = nums[i];
            double b = nums[j];

            // 加法
            next_nums.push_back(a + b);
            if (canGet24(next_nums)) return true;
            next_nums.pop_back();

            // 减法
            next_nums.push_back(a - b);
            if (canGet24(next_nums)) return true;
            next_nums.pop_back();

            // 乘法
            next_nums.push_back(a * b);
            if (canGet24(next_nums)) return true;
            next_nums.pop_back();

            // 除法
            if (b != 0) {
                next_nums.push_back(a / b);
                if (canGet24(next_nums)) return true;
                next_nums.pop_back();
            }
        }
    }

    return false;
}