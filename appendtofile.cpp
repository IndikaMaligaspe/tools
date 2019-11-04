#include <iostream>
#include <vector>
#include <string.h>
//#include <boost/filesystem.hpp>
#include <sys/types.h>
#include <dirent.h>

using namespace std;

vector<string> checkInput(int argv , char* argc[])
{
  string directory, include , append , exclude;
  vector<string> args;
  for(int i = 0; i < argv ; i++){
    string elem (argc[i]);
    int len = elem.size();
    size_t pos;
    if(( pos = elem.find("--directory=")) !=string::npos){
      directory = elem.substr(12, elem.size());
    }else if(( pos = elem.find("--include=")) != string::npos){
     include  = elem.substr(10 , elem.size());
    }else if(( pos = elem.find("--append=")) != string::npos) {
     append = elem.substr(9 , elem.size());
    }else if(( pos = elem.find("--exclude=")) != string::npos){
     exclude = elem.substr(10 , elem.size());
    }
  }

  if((directory.size()== 0 ) || (include.size() == 0)){
    return args;
  }else {
    args.push_back(directory);
    args.push_back(include);
    args.push_back(append);
    args.push_back(exclude);
    return args;
  }
}

vector<string> getFilesInFolder(string directory)
{
  vector<string> fileList;
  DIR* dirp = opendir(directory.c_str());
  struct dirent* dp;
  while((dp = readdir(dirp)) != NULL){
    string file;
    file = file.append(directory);
    file = file.append("/");
    file = file.append(dp->d_name);
    fileList.push_back(file);
  }
  return fileList;
}

bool appendToFile(vector<string> files , string path , string include , string append)
{
  FILE *file;
  int fileCount = files.size();
  for (string name : files){
    file = fopen(name.c_str() , "a");
    if(file == NULL){
       cout<<"File could not be opened ->"<<name<<endl;
       continue;        
    }else{
      cout<<"appending ...."<<append<<" to ->"<<name<<endl;
        fputs(append.c_str(),file);
        fputs("\r",file);
    }
    fclose(file);  
  }
  
}

void usage()
{
   cout<<"Usage appendtofile --directory=<directory> --include=<include file extenssion> --exclude=<exclude file extenssion>  --appnedstr=<string to append or empty for \\r>"<<endl;
}

int main(int argv , char* argc[])
{
 vector<string> args = checkInput(argv , argc);
 bool done = false;

 if ((argv <=1) || (args.empty()))
  {
   usage();
   return 0;
  }
  vector<string> fileNames = getFilesInFolder(args.at(0));
  if (!fileNames.empty())
    done = appendToFile(fileNames , args.at(0) , args.at(1) , args.at(2));
  return 0;
}
