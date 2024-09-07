#include "gtest/gtest.h"

#include "utils/time.h"

// 测试 check_date 函数
TEST(DateCheckTest, ValidDate) {
    EXPECT_TRUE(check_date(2023, 9, 5)) << "Valid date should return true.";
    EXPECT_FALSE(check_date(2023, 13, 5)) << "Invalid month should return false.";
    EXPECT_FALSE(check_date(2023, 9, 31)) << "Invalid day should return false.";
    EXPECT_FALSE(check_date(2023, 2, 30)) << "Invalid day for February should return false.";
    EXPECT_TRUE(check_date(2024, 2, 29)) << "Leap year date should return true.";
}

// 测试 string_to_data 函数
TEST(DateConversionTest, StringToDataValid) {
    int encoded_date = 0;
    EXPECT_NE(string_to_data("2023-09-05", encoded_date), -1) << "Valid date should not encode to -1.";
    
    int year, month, day;
    day = encoded_date % 100;            // Extract day
    encoded_date /= 100;
    month = encoded_date % 100;          // Extract month
    encoded_date /= 100;
    year = encoded_date;                 // Extract year
    
    EXPECT_EQ(year, 2023);
    EXPECT_EQ(month, 9);
    EXPECT_EQ(day, 5);
}

TEST(DateConversionTest, StringToDataInvalid) {
    int encoded_date = 0;
    EXPECT_EQ(string_to_data("2023-13-01", encoded_date), -1) << "Invalid month should encode to 0.";
    EXPECT_EQ(string_to_data("2023-02-30", encoded_date), -1) << "Invalid day should encode to 0.";
}

// 测试 date_to_string 函数
TEST(DateConversionTest, DataToString) {
    int encoded_date = 0;
    string_to_data("2023-09-05", encoded_date);
    std::string decoded_str = date_to_string(encoded_date);
    EXPECT_EQ(decoded_str, "2023-09-05") << "Decoded date should match original date string.";
    
    string_to_data("2024-02-29", encoded_date);
    decoded_str = date_to_string(encoded_date);
    EXPECT_EQ(decoded_str, "2024-02-29") << "Decoded date should match original leap year date string.";
}


int main(int argc, char **argv)
{
  // 分析gtest程序的命令行参数
  testing::InitGoogleTest(&argc, argv);

  // 调用RUN_ALL_TESTS()运行所有测试用例
  // main函数返回RUN_ALL_TESTS()的运行结果
  return RUN_ALL_TESTS();
}