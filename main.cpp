#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <iconv.h>
#include <sstream>
#include <vector>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#endif

using namespace std;

class ErrorInfo
{
public:
    //error_code
    //-1 复制文件时，文件打开失败
    //-2 目标文件夹创建失败
    //-3 源文件夹不存在
    //-4 已存在同名目标文件，且无法自动重命名
    //-5 源文件夹不存在或不是文件夹
    int error_code=0;
    string error_info="";
    ErrorInfo()
    {
        this->error_code = 0;
        this->error_info = "";
    }
    ErrorInfo& operator = (const ErrorInfo& a)
    {
        this->error_code = a.error_code;
        this->error_info = a.error_info;
        return *this;
    }
};

class ParseArgs
{
public:
    //使用keys保存传入的key，例如-i,-o,-g等等一些列以-开头的关键字
    //使用keys的第一个位置接受没有设定是什么的默认参数
    vector<string> keys;
    //保存对应关键字后边的参数
    vector<vector<string>> args;
    ParseArgs(int argc, char *argv[])
    {
        this->keys.push_back("default");
        this->args.push_back(vector<string>());
        //第一个是可执行文件，不需要
        for(int i=1;i<argc;i++)
        {
            //如果是key，就保存到key里面
            if(argv[i][0]=='-')
                this->keys.push_back(argv[i]);
            else
            {
                while(this->args.size()<this->keys.size())
                    this->args.push_back(vector<string>());
                this->args[this->keys.size()-1].push_back(argv[i]);
            }
        }
    }
};

unsigned long long UTF8ToGBK(char* input, size_t charInPutLen, char* output, size_t charOutPutLen)
{
    unsigned long long  ret=0;
    iconv_t cd;
    cd = iconv_open("GBK","utf-8");
    ret = iconv(cd, &input, &charInPutLen, &output, &charOutPutLen);
    iconv_close(cd);
    return ret;
}

unsigned long long UTF8ToGBK(const string& input, string& output)
{
    unsigned long long ret=0;
    char *t_output, *t_output_copy;
    int t_output_size = input.size()*2;
    t_output = (char *) malloc(t_output_size);
    t_output_copy = t_output;
    memset(t_output, 0, t_output_size);
    ret = UTF8ToGBK((char *)input.c_str(), input.size(), t_output, t_output_size);
    output = t_output;
    // iconv会改变t_output
    free(t_output_copy);
    return ret;
}

unsigned long long GBKToUTF8(char* input, size_t charInPutLen, char* output, size_t charOutPutLen)
{
    unsigned long long ret = 0;
    iconv_t cd;
    cd = iconv_open("utf-8", "GBK");
    ret = iconv(cd, &input, &charInPutLen, &output, &charOutPutLen);
    iconv_close(cd);
    return ret;
}

unsigned long long GBKToUTF8(const string& input, string& output)
{
    unsigned long long ret=0;
    char *t_output, *t_output_copy;
    int t_output_size = input.size()*2;
    t_output = (char *) malloc(t_output_size);
    t_output_copy = t_output;
    memset(t_output, 0, t_output_size);
    ret = GBKToUTF8((char *)input.c_str(), input.size(), t_output, t_output_size);
    output = t_output;
    // iconv会改变t_output
    free(t_output_copy);
    return ret;
}

inline void WIN32_UTF8_to_GBK(string &s)
{
#ifdef _WIN32
    //在windows下先把路径的字符从UTF8转GBK
    //如果转换失败ret会为-1，那么就保持源路径字符串不变，认为以及是适用于windows的GBK编码了
    unsigned long long ret = -1;
    string _t_str = "";
    ret = UTF8ToGBK(s, _t_str);
    if(ret!=(unsigned long long)(-1))s = _t_str;
#endif
}

bool check_file_exist(string file_path)
{
    WIN32_UTF8_to_GBK(file_path);
    //判断文件或文件夹是否存在
    return access(file_path.c_str(), 0)==0;
}

int get_FileType(string path)
{
    WIN32_UTF8_to_GBK(path);
    int FileType = -1;
#ifdef _WIN32
    intptr_t handle;
    _finddata_t findData;
    handle = _findfirst(path.c_str(), &findData);
    if (handle != -1)        // 检查是否成功
        FileType = findData.attrib;
    _findclose(handle);
#else
    struct stat _stat;
    int ret = lstat(path.c_str(), &_stat);
    if(ret!=-1)
        FileType = _stat.st_mode;
#endif
    return FileType;
}

bool is_FileType_means_Folder(int file_type)
{
#ifdef _WIN32
    return file_type == _A_SUBDIR;
#else
    return S_ISDIR(file_type);
#endif
}

int generate_makedir(string path)
{
    int t = -1;
#ifdef _WIN32
    t = mkdir(path.c_str());
#else
    t = mkdir(path.c_str(), 00766);
#endif
    return t;
}

bool is_same_file(string file1_path, string file2_path)
{
    WIN32_UTF8_to_GBK(file1_path);
    WIN32_UTF8_to_GBK(file2_path);
    bool flag = true;
    fstream in1, in2;

    in1.open(file1_path, ios::in|ios::binary);
    in2.open(file2_path, ios::in|ios::binary);

    if(in1.is_open()&&in2.is_open())
    {
        in1.seekg(0, ios::end);
        in2.seekg(0, ios::end);
        unsigned long long size_in1 = in1.tellg();
        unsigned long long size_in2 = in2.tellg();
        char c1, c2;
        if(size_in1!=size_in2)flag = false;
        else
        {
            in1.seekg(0, ios::beg);
            in2.seekg(0, ios::beg);

            while((!in1.eof())&&(!in2.eof()))
            {
                c1 = in1.get();
                c2 = in2.get();
                if(c1!=c2)
                {
                    flag = false;
                    break;
                }
            }
        }

    }
    else flag= false;

    in1.close();
    in2.close();

    return flag;
}

ErrorInfo copy_file(string src_file_path, string dest_file_path)
{
    ErrorInfo errorInfo;
    WIN32_UTF8_to_GBK(src_file_path);
    WIN32_UTF8_to_GBK(dest_file_path);
    FILE *infile, *outfile;
    infile = fopen(src_file_path.c_str(), "rb");
    outfile = fopen(dest_file_path.c_str(), "wb");
    if(infile== nullptr||outfile== nullptr)
    {
        fclose(infile);
        fclose(outfile);
        errorInfo.error_code = -1;
        if(infile== nullptr)
            errorInfo.error_info+=("源文件打开失败: "+src_file_path);
        if(outfile== nullptr)
            errorInfo.error_info+=("目标文件打开失败: "+dest_file_path);
        return errorInfo;
    }

    const int buff_size = 2048;
    char buff[buff_size];
    int read_size = 0;
    while(!feof(infile))
    {
        read_size = fread(buff, 1, buff_size, infile);
        if(read_size>0)
            fwrite(buff, 1, read_size, outfile);
    }
    fclose(infile);
    fclose(outfile);
    return errorInfo;
}

vector<string> get_folder_sub_files(string folder_path)
{
    WIN32_UTF8_to_GBK(folder_path);
    vector<string> sub_files;
    //首先检查路径十分正确
    int folder_file_type = get_FileType(folder_path);
    if(!is_FileType_means_Folder(folder_file_type))return sub_files;

#ifdef _WIN32
    //获取文件夹下的文件
    //文件句柄
    //注意：我发现有些文章代码此处是long类型，实测运行中会报错访问异常
    intptr_t hFile = -1;
    //文件信息
    struct _finddata_t file_data;
    if ((hFile = _findfirst(string(folder_path+"\\*").c_str(), &file_data)) != -1)
    {
        do
        {
            //如果不是,把文件绝对路径存入vector中
            if(strcmp(file_data.name, ".") != 0 && strcmp(file_data.name, "..") != 0)
            {
                sub_files.push_back(file_data.name);
            }
        } while (_findnext(hFile, &file_data) == 0);
    }
    close(hFile);
#else
    DIR * _dir;
    struct dirent *_dirrent;
    _dir = opendir(folder_path.c_str());
    if (_dir)
    {
        while (_dirrent = readdir(_dir))
        {
            if(strcmp(_dirrent->d_name, ".") != 0 && strcmp(_dirrent->d_name, "..") != 0)
            {
                sub_files.push_back(_dirrent->d_name);
            }
        }
    }
    closedir(_dir);
#endif
    return sub_files;
}

ErrorInfo merge_folder(string input_folder_path, string merge_folder_path)
{
    ErrorInfo errorInfo;
    WIN32_UTF8_to_GBK(input_folder_path);
    WIN32_UTF8_to_GBK(merge_folder_path);
    //要合并入的文件路径有下面3种情况
    //1 merge_folder是一个已有的文件夹，那么就不用处理，等待合并即可
    //2 merge_folder是一个还未创建的文件夹路径，试着创建文件夹
    //3 merge_folder是一个错误路径，无法创建文件夹
    int merge_folder_file_type = get_FileType(merge_folder_path);
    if(!is_FileType_means_Folder(merge_folder_file_type))
    {
        int t = generate_makedir(merge_folder_path);
        if(t==-1)
        {
            errorInfo.error_code = -2;
            errorInfo.error_info += ("目标文件夹创建失败: "+merge_folder_path);
            return errorInfo;
        }
    }

    //确定输入文件夹路径正确
    int input_folder_file_type = get_FileType(input_folder_path);
    //首先判断两个源文件夹是否存在
    if(!is_FileType_means_Folder(input_folder_file_type))
    {
        errorInfo.error_code = -3;
        errorInfo.error_info += ("源文件夹不存在: "+input_folder_path);
        return errorInfo;
    }

    //首先判断两个文件夹是否是同一个文件夹,若是同一个文件夹，那就直接返回，不用合并了
    if(input_folder_path==merge_folder_path)return errorInfo;
    //否则
    //分别获取源文件夹下的所有文件
    vector<string> input_folder_sub_files = get_folder_sub_files(input_folder_path);

    //遍历源文件夹的所有文件
    string input_file_path="";
    string merge_file_path="";

    for(int i=0;i<input_folder_sub_files.size();i++)
    {
        //判断源文件是文件还是文件夹，如果是文件夹直接递归调用merge_folder即可
        //merge_folder可以自己处理好目标文件夹的问题
        //如果是文件，那么要判断是否在merge_folder存在同名文件（不是文件夹）
        input_file_path = input_folder_path+"/"+input_folder_sub_files[i];
        merge_file_path = merge_folder_path+"/"+input_folder_sub_files[i];

        if(is_FileType_means_Folder(get_FileType(input_file_path)))
        {
            ErrorInfo t_errorInfo=merge_folder(input_file_path, merge_file_path);
            if(t_errorInfo.error_code!=0)
            {
                errorInfo = t_errorInfo;
                return errorInfo;
            }
        }
        else
        {
            //如果这个文件在目标文件夹中有同名文件(不是文件夹)，那么对比两个文件是否相同，相同就跳过，否则加后缀重命名
            if(check_file_exist(merge_file_path) && !is_FileType_means_Folder(get_FileType(merge_file_path)))
            {
                //如果这个同名文件相同，就啥也不干，否则尝试重命名为一个新文件名，让复制过去
                if(!is_same_file(input_file_path, merge_file_path))
                {
                    //有同名文件，并且文件内容不同，重命名并复制过去
                    const int max_try = 1000;
                    string new_name = "";
                    string new_merge_file_path = "";
                    bool new_name_flag = false;
                    for(int j=1;j<=max_try;j++)
                    {
                        new_name = to_string(j)+"_"+input_folder_sub_files[i];
                        new_merge_file_path = merge_folder_path+"/"+new_name;
                        //如果不存在这个文件，那么就可以用这个名字
                        //否则要判断用这个名字的是一个文件夹还是文件
                        if(check_file_exist(new_merge_file_path))
                        {
                            //如果存在这个名字的文件，但是这个文件是一个文件夹，那么我们还是可以用这个新名字创建一个文件的
                            if(is_FileType_means_Folder(get_FileType(new_merge_file_path)))
                            {
                                new_name_flag = true;
                                break;
                            }
                        }
                        else{new_name_flag = true;break;}
                    }
                    //如果找到了一个合适的名字，就复制这个文件为一个新名字过去，否则返回错误
                    if(new_name_flag)
                    {
                        merge_file_path = new_merge_file_path;
                        ErrorInfo t_errorInfo = copy_file(input_file_path, merge_file_path);
                        if(t_errorInfo.error_code!=0)
                        {
                            errorInfo = t_errorInfo;
                            return errorInfo;
                        }
//                    else merge_folder_sub_files_set.insert(new_name);//没必要
                    }
                    else
                    {
                        errorInfo.error_code = -4;
                        errorInfo.error_info += ("已存在同名目标文件，且无法自动重命名: "+input_folder_sub_files[i]);
                        return errorInfo;
                    }
                }
            }
            else
            {
                //目标文件夹中不存在这个文件(可以存在这个名字的文件夹)，那么复制过去
                input_file_path = input_folder_path+"/"+input_folder_sub_files[i];
                merge_file_path = merge_folder_path+"/"+input_folder_sub_files[i];
                ErrorInfo t_errorInfo = copy_file(input_file_path, merge_file_path);
                if(t_errorInfo.error_code!=0)
                {
                    errorInfo = t_errorInfo;
                    return errorInfo;
                }
            }
        }
    }
    return errorInfo;
}

ErrorInfo merge_folder(string input_folder1_path, string input_folder2_path, string merge_folder_path)
{
    ErrorInfo errorInfo;
    WIN32_UTF8_to_GBK(input_folder1_path);
    WIN32_UTF8_to_GBK(input_folder2_path);
    WIN32_UTF8_to_GBK(merge_folder_path);
    //要合并入的文件路径有下面3种情况
    //1 merge_folder是一个已有的文件夹，那么就不用处理，等待合并即可
    //2 merge_folder是一个还未创建的文件夹路径，试着创建文件夹
    //3 merge_folder是一个错误路径，无法创建文件夹
    int merge_folder_file_type = get_FileType(merge_folder_path);
    if(!is_FileType_means_Folder(merge_folder_file_type))
    {
        int t = generate_makedir(merge_folder_path);
        if(t==-1)
        {
            errorInfo.error_code = -2;
            errorInfo.error_info += ("目标文件夹创建失败: "+merge_folder_path);
            return errorInfo;
        }
    }

    //确定两个输入路径皆为正确的文件夹路径
    int input_folder1_file_type = get_FileType(input_folder1_path);
    int input_folder2_file_type = get_FileType(input_folder2_path);

    //首先判断两个源文件夹是否存在
    if(is_FileType_means_Folder(input_folder1_file_type)&& is_FileType_means_Folder(input_folder2_file_type))
    {
        //先去合并第一个文件夹和merge文件夹
        merge_folder(input_folder1_path, merge_folder_path);
        //再合并第二个文件夹和merge文件夹
        merge_folder(input_folder2_path, merge_folder_path);
    }
    else
    {
        errorInfo.error_code = -5;
        string t = "源文件夹不存在或不是文件夹: ";
        WIN32_UTF8_to_GBK(t);
        if(!is_FileType_means_Folder(input_folder1_file_type))
            errorInfo.error_info += (t+input_folder1_path);
        if(!is_FileType_means_Folder(input_folder2_file_type))
            errorInfo.error_info += (t+input_folder2_path);
        return errorInfo;
    }
    return errorInfo;
}

bool parse_args(int argc, char *argv[], string &input_folder1_path, string &input_folder2_path, string &merge_folder_path)
{
    ParseArgs parseArgs(argc, argv);
    //首先判断是否有我们需要的-i的参数和-o的参数，如果有就先设定，最后使用默认参数设定缺省的值

    //判断-i
    for(int i=1;i!=parseArgs.keys.size();i++)
    {
        if(parseArgs.keys[i]=="-i")
        {
            if(parseArgs.args[i].size()==2)
            {
                input_folder1_path = parseArgs.args[i][0];
                input_folder2_path = parseArgs.args[i][1];
            }
            else if(parseArgs.args[i].size()==1)
            {
                input_folder1_path = parseArgs.args[i][0];
            }else return false;
        }
    }

    //判断-o
    for(int i=1;i!=parseArgs.keys.size();i++)
    {
        if(parseArgs.keys[i]=="-o")
        {
            if(parseArgs.args[i].size()==1)
                merge_folder_path = parseArgs.args[i][0];
            else return false;
        }
    }

    int cnt = 0;

    if(cnt<parseArgs.args[0].size()&&input_folder1_path.size()<=0)
    {
        input_folder1_path = parseArgs.args[0][cnt];
        cnt++;
    }

    if(cnt<parseArgs.args[0].size()&&(merge_folder_path.size()>0||cnt+1<parseArgs.args[0].size()))
    {
        input_folder2_path = parseArgs.args[0][cnt];
        cnt++;
    }

    if(cnt<parseArgs.args[0].size()&&merge_folder_path.size()<=0)
    {
        merge_folder_path = parseArgs.args[0][cnt];
        cnt++;
    }
    return true;
}

int main(int argc,char *argv[]) {
    string help = "usage: FolderMerge -i <folder1_path> [folder2_path] -o <merge_folder_path>";
    string input_folder1_path = "";
    string input_folder2_path = "";
    string merge_folder_path = "";
    bool flag = parse_args(argc, argv, input_folder1_path, input_folder2_path, merge_folder_path);
    if(flag&&input_folder1_path.size()>0&&merge_folder_path.size()>0)
    {
        ErrorInfo errorInfo;
        if(input_folder2_path.size()>0)
            errorInfo = merge_folder(input_folder1_path, input_folder2_path, merge_folder_path);
        else
            errorInfo = merge_folder(input_folder1_path, merge_folder_path);

        if(errorInfo.error_code==0)
        {
            string t = "文件夹合并完成!";
            WIN32_UTF8_to_GBK(t);
            cout<<t<<endl;
        }
        else
        {
            string t[3];
            t[0] = "有错误发生!";
            t[1] = "错误代码: ";
            t[2] = "错误信息: ";
            for(int i=0;i!=3;i++)
            {
                WIN32_UTF8_to_GBK(t[i]);
            }
            cout<<t[0]<<endl;
            cout<<t[1]<<errorInfo.error_code<<endl;
            cout<<t[2]<<errorInfo.error_info<<endl;
        }
    }
    else
    {
        cout<<help<<endl;
    }
    return 0;
}
