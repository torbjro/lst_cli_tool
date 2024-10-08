#include <iostream>
#include <array>
#include "string"
#include "filesystem"
#include "ostream"
#include "vector"
#include "sstream"

inline void print_indentation(int depth) {
    for (int i = 0; i < depth; ++i) {
        std::cout << "     |";
    }
}

inline void print_dir(const std::string* dir, const int max_depth, int depth) {
    print_indentation(depth);
    std::cout << "-- " << *dir;

    if (depth == max_depth - 1) {
        std::cout << " (.)";
    }
    std::cout << std::endl;
}

inline void print_file(const std::string* file, const int depth) {
    print_indentation(depth);
    std::cout << "-- " << *file << std::endl;
}

std::string exec(const char* cmd, std::string res) {
    std::array<char, 128> buf;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe)
        throw std::runtime_error("[ERROR]: popen failed. cmd: " + std::string(cmd) + ".\n");
    while (fgets(buf.data(), buf.size(), pipe.get()) != nullptr) {
        res += buf.data();
    }
    return res;
}

std::vector<std::string> get_ls(const std::string& path, int max_depth, int cur_depth = 0) {
    // base case
    if (max_depth == cur_depth)
        return {};

    // get ls output
    std::string cmd = "ls -l " + path;
    std::string raw_res;
    raw_res = exec(cmd.c_str(), raw_res);

    // split full output of cmd into files and directories
    std::vector<std::string> res;
    std::istringstream tokenStream(raw_res);
    std::string line;
    std::getline(tokenStream, line); // skip first line
    int lim = 0;
    while (std::getline(tokenStream, line)) {
        std::stringstream lineStream(line);
        std::vector<std::string> tokens;
        std::string token;

        while (lineStream >> token)
            tokens.push_back(token);

        if (!token.empty() && tokens[0][0] == 'd') {
            // need to slice vector to account for dir names with spaces
            std::vector<std::string> tokens_name = std::vector<std::string>(tokens.begin() + 8, tokens.end());
            std::string dir_name;
            int c = 1;
            for (const std::string& dir_part : tokens_name) {
                dir_name += dir_part;
                if (c < tokens_name.size())
                    dir_name += " ";
                c++; // pun intended
            }
            std::string full_path = path;
            full_path.append("/");
            full_path.append(dir_name);
            print_dir(&dir_name, max_depth, cur_depth);
            res.push_back(dir_name);
            // TODO: bug when dir name has space in it, cannot do ls on it
            std::vector<std::string> sub_dir = get_ls(full_path, max_depth, ++cur_depth);
            --cur_depth;
            res.insert(res.end(), sub_dir.begin(), sub_dir.end());
        }
        else {
            std::vector<std::string> tokens_name = std::vector<std::string>(tokens.begin() + 8, tokens.end());
            std::string file_name;
            int c = 1;
            for (const std::string& file_part : tokens_name) {
                file_name += file_part;
                if (c < tokens_name.size())
                    file_name += " ";
                c++;
            }
            print_file(&file_name, cur_depth);
            ++lim;
            res.push_back(file_name);
        }

        if (lim > 10) {
            print_indentation(cur_depth);
            std::cout << "   ..." << std::endl;
            break;
        }
    }
    return res;
}

int main(int argc, char *argv[]) {
    std::string start_dir;
    if (argc == 1) {
        start_dir = std::filesystem::current_path();
    } else
        start_dir = argv[1];
    std::cout<< "\n" << start_dir <<" contents:\n" << std::endl;
    int max_depth = 4;
    std::vector<std::string> ls = get_ls(start_dir, max_depth, 0);
    std::cout << "\n" << std::endl;
    return 0;
}
