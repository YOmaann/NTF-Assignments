/*
  A very dumb proxy caching scheme. Save everything. Doesn't implement file locking. Not scalable.
 */
#include <sys/stat.h>
#include <sys/types.h>
#include "socketp.h"
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

std::string cache::sanitize(std::string i) {
 if(i.substr(0, 7) == "http://" || i.substr(0, 8) == "https://")
   i = i.substr(7);
 std::replace(i.begin(), i.end(), ':', '_');
 std::string o = "";

 for(int j = 0; j < i.length(); j++) {
   if(i[j] == '?' || i[j] == '#')
     break;
   o = o + i[j];
 }

 return o;
}
std::vector<std::string> cache::adv_tokenizer(std::string s, char del)
{
 std::stringstream ss(s);
 std::string word;
 std::vector<std::string> out;

 while (!ss.eof()) {
   std::getline(ss, word, del);
   out.push_back(word);
 }
 
 return out;
}

std::string cache::recur_dir(std::vector<std::string> paths) {
 std::string path = "./cache";
 struct stat sb;

 for(int i = 0; i < paths.size() - 1; i++) {

   std::string dir = paths[i];
   if(dir == "") continue;
   //   if(dir.find(".") != std::string::npos) break;
   path = path + "/" + dir;
   if(stat(path.c_str(), &sb) != 0)
     if(mkdir(path.c_str(), 0777) < 0)
       throw CacheError("Cannot create cache folder!");
   //std::cout << "debug :" << path << std::endl;
 }

 return path;
}
bool cache::check_cache(std::string file) {
  file = "./cache/" + sanitize(file) + ".txt";

  
  struct stat sb;

  if(stat(file.c_str(), &sb) == 0 && !(sb.st_mode & S_IFDIR))
    return true;
  return false;
}
std::vector<std::string> cache::get_cache(std::string file) {
  file = "./cache/" + sanitize(file) + ".txt";
  
  std::cout << "Cached data : " << file << std::endl;

  // Reading file and outputting
  std::vector<std::string> out;
  std::ifstream s(file);
  std:: string str;

  int line = 0;
  while(std::getline(s, str)) {
    //    out = out + str;
    out.push_back(str);
    //std::cout << "Cached data" << line << " : " << str << std::endl;
    line++;
  
  }
    //  std::cout << "Cached data 2: " << out << std::endl;
  s.close();

  //std::cout << "Cached data : " << file << std::endl;
  //std::cout << out << std::endl;
  return out;
}

void cache::create_cache(std::string url, std::string contents){
   struct stat sb;
   std::string file = sanitize(url);

   std::cout << "File : " << file << std::endl;
   auto paths = adv_tokenizer(file, '/');
   
   std::string path_o = recur_dir(paths);
   std::string out = paths[paths.size() - 1] + ".txt";

   out = path_o + "/" + out;
   std::cout << "Writing cache to :  " << out << std::endl;
   // creating file to cache
   std::ofstream(out) << contents;
}


