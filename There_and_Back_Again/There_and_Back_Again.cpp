#include <iostream>
#include <string>
#include <omp.h>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>

std::string func(std::string word) {

   std::string result;

   for (int i = 0; i < word.length(); i++) {
      if (isalpha((unsigned char)word[i])) {
         result += word[i];
      }
      else if (word[i] == '-' && i > 0 && i < word.length() - 1 && isalpha((unsigned char)word[i - 1]) && isalpha((unsigned char)word[i + 1])) {
         result += word[i];
      }
   }

   return result;
}
int main()
{
   setlocale(LC_ALL, "Russian");
   std::ifstream file("D:/virandfpc/fpc/There_and_Back_Again/0a0ffa5fb2b236de.txt");
   if (!file.is_open()) {
      std::cerr << "Error opening file." << std::endl;
      return 1;
   }

   std::map<std::string, int> wordFrequency;

#pragma omp parallel
   {
      std::string word;
      while (file >> word) {
#pragma omp critical
         {
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            word = func(word);
            //std::cout << word << std::endl;
            wordFrequency[word]++;
         }
      }
   }

   file.close();

   
   std::vector<std::pair<std::string, int>> sortedWords(wordFrequency.begin(), wordFrequency.end());
   std::sort(sortedWords.begin(), sortedWords.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
         return a.second > b.second;
      });

   for (const auto& pair : sortedWords) {
      std::cout << pair.first << " : " << pair.second << std::endl;
   }

}

