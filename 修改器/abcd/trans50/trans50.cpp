﻿// trans50.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
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
                case 9:
                {
                    str = std::format("x[{}] = string.format({}, {});", e2, e3, e_get(0, e1, e4));
                    break;
                }
                case 10:
                {
                    str = std::format("x[{}] = length(x[{}]);", e2, e1);
                    break;
                }
                case 11:
                {
                    str = std::format("x[{}] = x[{}]..x[{}];", e3, e1, e2);
                    break;
                }
                case 12:
                {
                    str = std::format("x[{}] = string.space({});", e2, e_GetValue(0, e1, e3));
                    break;
                }
                case 16:
                {
                    std::vector<std::string> names = { "role", "item", "submap", "magic", "shop" };
                    str = std::format("{}({}).data({}) = {};", names[e2], e_GetValue(0, e1, e3), e_GetValue(1, e1, e4), e_GetValue(2, e1, e5));
                    break;
                }
                case 17:
                {
                    std::vector<std::string> names = { "role", "item", "submap", "magic", "shop" };
                    str = std::format("x[{}] = {}({}).data({});", e5, names[e2], e_GetValue(0, e1, e3), e_GetValue(1, e1, e4));
                    break;
                }
                case 18:
                {
                    str = std::format("SetTeam({}, {});", e_GetValue(0, e1, e2), e_GetValue(0, e1, e3));
                    break;
                }
                case 19:
                {
                    str = std::format("x[{}] = GetTeam({});", e3, e_GetValue(0, e1, e2));
                    break;
                }
                case 20:
                {
                    str = std::format("x[{}] = GetItemAmount({});", e3, e_GetValue(0, e1, e2));
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
                case 23:
                {
                    str = std::format("sdata({}, {}, {}, {}) = {};", e_GetValue(0, e1, e2), e_GetValue(1, e1, e3), e_GetValue(2, e1, e4), e_GetValue(3, e1, e5), e_GetValue(4, e1, e6));
                    break;
                }
                case 24:
                {
                    str = std::format("x[{}] = sdata({}, {}, {}, {});", e6, e_GetValue(0, e1, e2), e_GetValue(1, e1, e3), e_GetValue(2, e1, e4), e_GetValue(3, e1, e5));
                    break;
                }
                case 25:
                {
                    str = std::format("write_mem(0x{:x} + {}, {});", e3 + e4 * 0x10000, e_GetValue(1, e1, e6),
                        e_GetValue(0, e1, e5));
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
                case 33:
                {
                    str = std::format("DrawString(x[{}], {}, {}, {});", e2, e_GetValue(0, e1, e3), e_GetValue(1, e1, e4), e_GetValue(2, e1, e5));
                    next_parameter = -1;
                    break;
                }
                case 34:
                {
                    str = std::format("DrawRect({}, {}, {}, {});", e_GetValue(0, e1, e2), e_GetValue(1, e1, e3), e_GetValue(2, e1, e4), e_GetValue(3, e1, e5));
                    break;
                }
                case 35:
                {
                    str = std::format("x[{}] = lib.GetKey();", e1);
                    break;
                }
                case 36:
                {
                    str = std::format("x[28672] = showmessage(x[{}], {}, {}, {});", e2, e_GetValue(0, e1, e3), e_GetValue(1, e1, e4), e_GetValue(2, e1, e5));
                    break;
                }
                case 37:
                {
                    str = std::format("lib.Delay({});", e_GetValue(0, e1, e2));
                    break;
                }
                case 38:
                {
                    str = std::format("x[{}] = math.random({});", e3, e_GetValue(0, e1, e2));
                    break;
                }
                case 39:
                case 40:
                {
                    str = "strs = {};\n";
                    str += std::format("for i=1, {} do\n", e_GetValue(0, e1, e2));
                    str += std::format("strs[i] = x[x[{}] + i - 1];\n", e3);
                    str += "end\n";
                    str += std::format("x[{}] = menu(strs, #strs, {}, {});", e4, e_GetValue(1, e1, e5), e_GetValue(2, e1, e6));
                    break;
                }
                case 41:
                {
                    //画图
                    if (e2 == 0)
                    {
                        str = std::format("DrawMainImage({} / 2, {}, {});", e_GetValue(2, e1, e5), e_GetValue(0, e1, e3), e_GetValue(1, e1, e4));
                    }
                    else if (e2 == 1)
                    {
                        str = std::format("DrawMainImage({} / 2, {}, {});", e_GetValue(2, e1, e5), e_GetValue(0, e1, e3), e_GetValue(1, e1, e4));
                    }
                }
                }
            }
            str = std::format("{}{}        --{}", std::string(0, ' '), str, line.substr(pos));
        }
        else
        {
            auto pos_l = line.find("(");
            auto pos_r = line.find(")");
            if (pos_l != std::string::npos && pos_r != std::string::npos && pos_l < pos_r)
            {
                std::string params = line.substr(pos_l + 1, pos_r - pos_l - 1);
                std::vector<std::string> param_list = strfunc::splitString(params, ",", true);
                if (next_parameter >= 0 && next_parameter < param_list.size())
                {
                    param_list[next_parameter] = "9999999";
                    next_parameter = -1;
                }
                auto not_space = line.find_first_not_of(" \t");
                str = line.substr(not_space, pos_l - not_space + 1);
                for (int i = 0; i < param_list.size(); i++)
                {
                    std::string param = param_list[i];
                    str += param;
                    if (i != param_list.size() - 1)
                    {
                        str += ", ";
                    }                    
                }
                str += line.substr(pos_r);
            }
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
    if (argc > 1)
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
