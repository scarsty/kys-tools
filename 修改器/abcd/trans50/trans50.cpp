// trans50.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include <string>

#include "GrpIdxFile.h"
#include "PotConv.h"
#include "filefunc.h"
#include "strfunc.h"
#include <format>
#include <print>
#include <vector>

void trans50(std::string file, std::string talk_path)
{
    std::string result;

    std::string content = filefunc::readFileToString(file);
    strfunc::replaceAllSubStringRef(content, "\r", "");
    std::vector<std::string> lines = strfunc::splitString(content, "\n", false);

    int next_parameter = -1;
    auto e_get = [&next_parameter](int bit, int t, int x) -> std::string
    {
        int i = t & (1 << bit);
        if (x == 9999999)
        {
            return "temp";
        }
        if (i == 0)
        {
            return std::to_string(x);
        }
        else
        {
            return std::format("x[{}]", x);
        }
    };
    auto is_number = [](int bit, int t, int x)
    {
        return (t & (1 << bit)) == 0;
    };
    auto e_GetValue = e_get;
    //读对话
    std::vector<int> offset, length;
    auto talk = GrpIdxFile::getIdxContent(talk_path + "/talk.idx", talk_path + "/talk.grp", &offset, &length);
    for (int i = 0; i < offset.back(); i++)
    {
        if (talk[i])
        {
            talk[i] = talk[i] ^ 0xff;
        }
    }
    std::vector<std::string> talk_contents;
    for (int i = 0; i < length.size(); i++)
    {
        std::string str = strfunc::replaceAllSubString(PotConv::cp950toutf8(talk.data() + offset[i]), "*", "");
        talk_contents.push_back(str);
    }

    for (auto& line : lines)
    {
        std::string str = line;
        if (line.find("instruct_50") != std::string::npos)
        {
            auto pos = line.find("instruct_50");
            auto nums = strfunc::findNumbers<int>(line.substr(pos + 12));
            if (nums.size() >= 7)
            {
                if (next_parameter >= 1 && next_parameter <= nums.size())
                {
                    nums[next_parameter - 1] = 9999999;
                    next_parameter = -1;
                }
                int code = nums[0];
                int e1 = nums[1];
                int e2 = nums[2];
                int e3 = nums[3];
                int e4 = nums[4];
                int e5 = nums[5];
                int e6 = nums[6];

                switch (code)
                {
                case 0:
                    str = std::format("x[{}] = {};", e1, e2);
                    break;
                case 1:
                    str = std::format("x[{} + {}] = {};", e3, e_get(0, e1, e4), e_get(1, e1, e5));
                    break;
                case 2:
                    str = std::format("x[{}] = x[{}+ {}];", e5, e3, e_get(0, e1, e4));
                    break;
                case 3:
                {
                    std::string op = "+-*/%";
                    if (e2 >= 0 && e2 <= 4)
                    {
                        str = std::format("x[{}] = x[{}] {} {};", e3, e4, op[e2], e_get(0, e1, e5));
                    }
                    else if (e2 == 5)
                    {
                        str = std::format("x[{}] = uint(x[{}]) / {};", e3, e4, op[e2 - 4], e_get(0, e1, e5));
                    }
                    break;
                }
                case 4:
                {
                    std::vector<std::string> op = { "<", "<=", "==", "!=", ">=", ">" };
                    if (e2 >= 0 && e2 <= 5)
                    {
                        str = std::format("if x[{}] {} {} then jump_flag = false; else jump_flag = true; end;", e3, op[e2], e_get(0, e1, e4));
                    }
                    else if (e2 == 6)
                    {
                        str = "jump_flag = false;";
                    }
                    else if (e2 == 7)
                    {
                        str = "jump_flag = true;";
                    }
                    break;
                }
                case 5:
                {
                    str = "x = {};";
                    break;
                }
                case 6: break;
                case 7: break;
                case 8:
                {
                    if (is_number(0, e1, e2))
                    {
                        str = std::format("x[{}] = \"{}\";", e3, talk_contents[e2]);
                    }
                    else
                    {
                        str = std::format("x[{}] = talk({});", e3, e_get(0, e1, e2));
                    }
                    break;
                }
                case 10:
                {
                    str = std::format("x[{}] = length(x[{}]);", e2, e1);
                    break;
                }
                case 16:
                {
                    std::vector<std::string> names = { "role", "item", "submap", "magic", "shop" };
                    str = std::format("{}({}).data({}) = {};", names[e2], e_GetValue(0, e1, e3), e_GetValue(1, e1, e4), e_GetValue(2, e1, e5));
                    break;
                }
                case 21:
                {
                    str = std::format("ddata({}, {}, {}) = {};", e_GetValue(0, e1, e2), e_GetValue(1, e1, e3), e_GetValue(2, e1, e4), e_GetValue(3, e1, e5));

                    break;
                }
                case 22:
                {
                    str = std::format("x[{}] = ddata({}, {}, {});", e5, e_GetValue(0, e1, e2), e_GetValue(1, e1, e3), e_GetValue(2, e1, e4));
                    break;
                }
                case 26:
                {
                    str = std::format("x[{}] = read_mem(0x{:x} + {});", e5, e3 + e4 * 0x10000, e_GetValue(0, e1, e6));
                    break;
                }
                case 27:
                {
                    std::vector<std::string> names = { "role", "item", "submap", "magic", "shop" };
                    str = std::format("x[{}] = {}({}).name;", e4, names[e2], e_GetValue(0, e1, e3));
                    break;
                }
                case 32:
                {
                    str = std::format("temp = x[{}];", e2);
                    e_GetValue(0, e1, e3);
                    next_parameter = e3;
                    break;
                }
                case 34:
                {
                    str = std::format("DrawRect({}, {}, {}, {});", e_GetValue(0, e1, e2), e_GetValue(1, e1, e3), e_GetValue(2, e1, e4), e_GetValue(3, e1, e5));
                    break;
                }
                case 36:
                {
                    str = std::format("x[28672] = showmessage(x[{}], {}, {}, {});", e2, e_GetValue(0, e1, e3), e_GetValue(1, e1, e4), e_GetValue(2, e1, e5));
                    break;
                }
                case 39:
                case 40:
                {
                    str = "strs = {};\n";
                    str += std::format("for i=1,{} do\n", e_GetValue(0, e1, e2));
                    str += std::format("strs[i] = x[x[{}]+i-1];\n", e3);
                    str += "end\n";
                    str += std::format("x[{}] = menu(strs, #strs, {}, {});", e4, e_GetValue(1, e1, e5), e_GetValue(2, e1, e6));
                    break;
                }
                }
            }
            str = std::format("{}{}        --{}", std::string(0, ' '), str, line.substr(pos));
        }
        strfunc::replaceOneSubStringRef(str, "9999999", "temp");
        strfunc::replaceOneSubStringRef(str, "CheckRoleSexual(256)", "jump_flag");
        printf("%s\n", str.c_str());
        result += str + "\n";
    }
    filefunc::writeStringToFile(result, "out.lua");
}

int main(int argc, char* argv[])
{
    if (argc>1)
    {
        trans50(argv[1], ".");
    }
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧:
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
