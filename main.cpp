#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <numeric>
#include <iterator>
#include <utility>
#include <cctype>
#include <fstream>
#include <sstream>
#include <cstring>
#include <set>
#include "main.h"
int CheckIfInt(std::string a) {
  // Function to check if a word entered can be turned to an int
  try {
    std::size_t pos{0};
    int i = std::stoi(a, &pos);
    return i;
  } catch (std::invalid_argument &) {
    return 0;
  }
}
std::string OnlyPoints(std::pair<std::string, std::string> kv) {
  // Function to be used to calculate overall points one can get in class.
  // don't want Labs, HW (As that will be calculated later), and Honors
  if (kv.first.find("Lab") == std::string::npos &&
      kv.first.find("HW") == std::string::npos &&
      kv.first.find("Honors") == std::string::npos) {
    return kv.second;
  }
  return "0";
}
bool CheckLabs(std::string a) {
  // Returns 1 or 0, based on if it can be turned to an int and isn't 1 (return
  // 1) or the lab has score of 1 (return 0)
  try {
    std::size_t pos{0};
    int i = std::stoi(a, &pos);
    if (i != 1) {
      return 1;
    } else {
      return 0;
    }
  } catch (std::invalid_argument &) {
    return 1;
  }
}

char FindRows(char a, std::string &the_str) {
  // Add Spaces to strings with a space between them (Ex. Final Exam)
  // Words that have no value, they will be found as ",,"in the string
  // Place some sort of symbol to be taken out later between the ",,"
  if (a == ' ') {
    return '-';
  }
  if (the_str.find(",,") != std::string::npos) {
    the_str.insert(the_str.find(",,") + 1, "=");
  }
  return a;
}
char MakeWords(char z, std::string &word, std::vector<std::string> &Row) {
  // Recreate the initial words (Ex. Final-Exam -> Final Exam)
  // Techincally this function is building up the vector of strings Row to be
  // used
  // Get rid of commas to make the words
  // If there is a space, this means it is at the end of the iterator and should
  // end
  if (z == '-') {
    word = word + ' ';
    return ' ';
  }
  if (z == ',') {
    Row.push_back(word);
    word = "";
    return ' ';
  }
  word = word + z;
  if (z == ' ') {
    Row.push_back(word);
    word = "";
    return ' ';
  }
  return z;
}
std::vector<std::string> MakeVector(std::string y) {
  // Add ',' to end so I can deal with scores with no values at the end of the
  // strings
  // This will make ",,", which FindRows will deal with
  // Trick is there as show, Row is what is actually being built up due to a
  // capture.
  y = y + ',';
  std::vector<std::string> Row;
  std::string word = "";
  std::string Trick = "";
  std::transform(y.begin(), y.end(), std::back_inserter(Trick),
                 [&word, &Row](auto z) { return MakeWords(z, word, Row); });
  return Row;
}
std::pair<std::string, std::string> MakeRow(std::string &x, std::string &y) {
  // Replace the symbol used between ",," earlier that was made to "=" once
  // commas were taken out to a value of ""
  // This function will make a pair of the first row to the all the other rows
  // Techincally it will make a pair of first row to first row (Which I delete
  // later)
  if (y.find("=") != std::string::npos || y.length() > 30) {
    y = "";
  }
  return std::make_pair(x, y);
}
std::vector<std::pair<std::string, std::string>> MakePair(
    std::vector<std::string> a, std::vector<std::vector<std::string>> Compare) {
  // This will make the pair and return a vector of pairs (each row)
  std::vector<std::pair<std::string, std::string>> pair;
  pair.reserve(Compare.at(0).size());
  std::transform(Compare.at(0).begin(), Compare.at(0).end(), a.begin(),
                 std::back_inserter(pair),
                 [](auto x, auto y) { return MakeRow(x, y); });
  return pair;
}
std::vector<std::string> FindID(
    std::vector<std::pair<std::string, std::string>> v) {
  // Used to find the ID of each student
  std::vector<std::string> I;
  std::pair<std::string, std::string> k = v.at(1);
  I.push_back(k.second);
  return I;
}
std::pair<std::string, std::map<std::string, std::string>> MakeMap(
    std::vector<std::pair<std::string, std::string>> v,
    std::vector<std::string> I) {
  // Takes the ID of each vector of pairs and create a ID to map of strings
  // Makes vector of pairs of strings to map of strings
  std::map<std::string, std::string> M;
  std::string id = I.at(0);
  std::copy(v.begin(), v.end(), std::inserter(M, M.begin()));
  return std::make_pair(id, M);
}
int AddVal(std::pair<std::string, std::string> Info, double &GPA, int &count) {
  // Checks each first value of pair and sees if it is a lab.
  // If it is a lab, it checks if the value is equal to 0
  // If it is equal to 0, a counter is added by 1.
  // If the counter reaches 3, the GPA is added by -0.5
  // count returns to 2 so if the count increases, GPA can continue decreasing
  // To make sure count stops, if the first value is Project, we have passed the
  // labs, hence no more counting.
  // Also returns the second value if it is possible to be turned to int, if not
  // returns 0
  if (Info.first.find("Lab") != std::string::npos) {
    if (Info.second == "0") {
      count += 1;
    }
  }
  if (count == 3) {
    GPA += -0.5;
    count = 2;
  }
  if (Info.first.find("Project") != std::string::npos) {
    count = 0;
  }
  try {
    std::size_t pos{0};
    int i = std::stoi(Info.second, &pos);
    return i;
  } catch (std::invalid_argument &) {
    return 0;
  }
}
std::pair<std::string, double> MakeGrade(
    std::pair<std::string, std::map<std::string, std::string>> a) {
  // Using transform, captures GPA and integer variable count that will be used
  // in AddVal.
  // Creates total Grade through GetPointTotalForStudent
  // Instead of lines filled with if statements to see if the grade is below or
  // above the threshold, I used maps to take the second double value and add it
  // to the captured GPA value.
  std::vector<int> G;
  double GPA = 0;
  int count = 0;
  std::transform(a.second.begin(), a.second.end(), std::back_inserter(G),
                 [&GPA, &count](auto b) { return AddVal(b, GPA, count); });
  int Grade = GetPointTotalForStudent(a.second);
  std::map<int, double> Points{{600, 1.0}, {650, 1.5}, {700, 2.0}, {750, 2.5},
                               {800, 3.0}, {850, 3.5}, {900, 4.0}, {950, 4.0}};
  if (Grade < 599) {
    GPA = 0;
  } else {
    GPA = GPA + Points.at((Grade / 50) * 50);
  }
  if (GPA > 4.0) {
    GPA = 4.0;
  }
  return std::make_pair(a.first, GPA);
}
std::string FindHonors(
    std::pair<std::string, std::map<std::string, std::string>> a, int minimum,
    std::set<std::string>& Eligible) {
  // Does the same thing as MakeGrade, but returns a string (The ID) if the
  // honors project value (The last value of Grade) is larger than minimum
  // Also if the GPA value is larger than 3.5 as that is the Honors requirement
  std::vector<int> G;
  double GPA = 0;
  int count = 0;
  std::string H;
  std::transform(a.second.begin(), a.second.end(), std::back_inserter(G),
                 [&GPA, &count](auto b) { return AddVal(b, GPA, count); });
  int Grade = std::accumulate(G.begin(), G.end(), 0) - G.back();
  std::map<int, double> Points{{600, 1.0}, {650, 1.5}, {700, 2.0}, {750, 2.5},
                               {800, 3.0}, {850, 3.5}, {900, 4.0}, {950, 4.0}};
  if (Grade < 599) {
    GPA = 0;
  } else {
    GPA = GPA + Points.at((Grade / 50) * 50);
  }
  if (GPA > 4.0) {
    GPA = 4.0;
  }
  if (G.back() >= minimum && GPA >= 3.5) {
    H = a.first;
    Eligible.insert(H);
  }
  return H;
}
int GetPointTotalForStudent(std::map<std::string, std::string> student_info,
                            std::string category) {
  // returns an int representing the number of points earned in total on a
  // particular type of assignment
  std::map<std::string, std::string> map_copy;
  std::copy_if(student_info.begin(), student_info.end(),
               std::inserter(map_copy, map_copy.end()), [category](auto a) {
                 return (a.first.find(category) != std::string::npos);
               });  // Copy Map with only the certain category keys and values
  std::vector<std::string> vals;
  std::transform(
      map_copy.begin(), map_copy.end(), std::back_inserter(vals),
      [](auto kv) { return kv.second; });  // Takes second values from the map
  std::vector<int> new_vals;
  std::transform(vals.begin(), vals.end(), std::back_inserter(new_vals),
                 [](auto a) {
                   return CheckIfInt(a);
                 });  // Makes sure those values are ints
  return std::accumulate(new_vals.begin(), new_vals.end(), 0);  // Returns sum
}
int GetPointTotalForStudent(std::map<std::string, std::string> student_info) {
  // returns the number of points earned by that student.
  std::vector<std::string> vals;
  std::map<std::string, std::string> HW_Vals;
  std::transform(student_info.begin(), student_info.end(),
                 std::back_inserter(vals), [](auto kv) {
                   return OnlyPoints(kv);
                 });  // Makes copy of map with only certain keys and values
  int HW_points = GetTopNHomeworkTotalForStudent(
      student_info,
      15);  // There are 17 HW's in all so I used top 15, 2 that are dropped
  std::vector<int> new_vals;
  std::transform(vals.begin(), vals.end(), std::back_inserter(new_vals),
                 [](auto a) {
                   return CheckIfInt(a);
                 });  // Makes sure those values are ints
  int Final =
      HW_points +
      std::accumulate(new_vals.begin(), new_vals.end(),
                      0);  // Get sum of HW and everything else worth points
  return Final;
}
int GetTopNHomeworkTotalForStudent(
    std::map<std::string, std::string> student_info, int tally) {
  // Returns Sum of top HW scores based on how large/small tally is
  std::map<std::string, std::string> map_copy;
  std::copy_if(student_info.begin(), student_info.end(),
               std::inserter(map_copy, map_copy.end()), [](auto a) {
                 return (a.first.find("HW") != std::string::npos);
               });  // Only HW keys/values
  std::vector<std::string> vals;
  std::transform(
      map_copy.begin(), map_copy.end(), std::back_inserter(vals),
      [](auto kv) { return kv.second; });  // Takes second values from the map
  std::vector<int> new_vals;
  std::transform(vals.begin(), vals.end(), std::back_inserter(new_vals),
                 [](auto a) {
                   return CheckIfInt(a);
                 });  // Makes sure those values are ints
  std::sort(new_vals.begin(), new_vals.end(), std::greater<>());
  if (tally > static_cast<int>(new_vals.size())) {  // Check if tally is larger
                                                    // than how many HW in
                                                    // general. Don't want to
                                                    // over-iterate.
    return std::accumulate(new_vals.begin(), new_vals.end(), 0);
  } else {
    return std::accumulate(new_vals.begin(), new_vals.begin() + tally, 0);
  }
}
int GetNumberOfMissingLabsForStudent(std::map<std::string,std::string> student_info){
    //Returns the number of labs that don't have a score of exactly 1
    std::map<std::string,std::string> map_copy;
    std::copy_if(student_info.begin(), student_info.end(), std::inserter(map_copy, map_copy.end()),[](auto a){return (a.first.find("Lab") != std::string::npos);}); //Get only lab key/values
    std::vector<std::string> vals;
    std::transform(map_copy.begin(),map_copy.end(),std::back_inserter(vals),
    [](auto kv){ return kv.second;}); //Takes second values from the map
    std::vector<int> new_vals;
    std::transform(vals.begin(),vals.end(),std::back_inserter(new_vals),[](auto a){return CheckLabs(a);}); //Returns a vector of 1 or 0 based on if string is equal to 0 or not an int
    return std::accumulate(new_vals.begin(),new_vals.end(),0); //return sum

}
std::map<std::string, std::map<std::string, std::string>> GetIDToInfoFromCSV(std::string filename){
    //This function returns a map of string (the ID) to student_info
    std::map<std::string,std::map<std::string,std::string>> id_to_student_info;
    std::ifstream input(filename); //Put file in ifstream
    std::string the_str (std::istreambuf_iterator<char>(input),(std::istreambuf_iterator<char>())); //Turns ifstream to stirng
    std::transform(the_str.begin(),the_str.end(),the_str.begin(),[&the_str](char a){return FindRows(a,the_str);}); //makes sure to fix values with no value and get rid of spaces
    std::stringstream iss(the_str); 
    std::vector<std::string> V {std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>()}; //Vector of string (each string is row)
    std::vector<std::vector<std::string>> Compare;
    std::transform(V.begin(),V.end(),std::back_inserter(Compare),[](auto & y){return MakeVector(y);}); //Make vector of rows (vector of words) and clean the strings
    std::vector<std::vector<std::pair<std::string,std::string>>> S;
    std::transform(Compare.begin(), Compare.end(), std::back_inserter(S),[Compare](auto a){return MakePair(a,Compare);});//Turns vector of vectors into a vector of vector of pairs (pair of the first row to everyy other row)
    S.erase(S.begin()); //Erase first vector as it is first row to first row
    std::vector<std::vector<std::string>> ID;
    std::transform(S.begin(),S.end(),std::back_inserter(ID),[](auto v){return FindID(v);}); //Makes vector of vectors of only the ID's
    std::transform(S.begin(),S.end(),ID.begin(),std::inserter(id_to_student_info, id_to_student_info.begin()),[](auto v, auto i){return MakeMap(v,i);}); //Mix the  vector of vector of pairs and vector of vectors of ID's together to make map of string ID and map of strings (key to value)
    return id_to_student_info;
}
std::map<std::string,double> GetIDToGrade(const std::map<std::string, std::map<std::string, std::string>> & id_to_student_info){
    // returns a map of string (ID) to double (the student’s calculated grade).
    std::map<std::string,double> id_to_grade;
    std::transform(id_to_student_info.begin(),id_to_student_info.end(),std::inserter(id_to_grade,id_to_grade.begin()),[](auto a){return MakeGrade(a);}); //Main transform that takes pair of map of strings and used the second value of map to calculate overall grade and GPA.
    return id_to_grade;
}
std::set<std::string> GetStudentsEligibleForHonorsCredit(const std::map<std::string, std::map<std::string, std::string>> & id_to_student_info, const int & minimum){
    // returns a set of strings (IDs) of the students that meet the honors requirements
    std::set<std::string> Eligible;
    std::set<std::string> Trick;
    std::transform(id_to_student_info.begin(),id_to_student_info.end(),std::inserter(Trick,Trick.begin()),[minimum,&Eligible](auto a){return FindHonors(a,minimum,Eligible);}); //takes pair of map of strings and used the second value of map to calculate overall grade and GPA, but only checks if GPA is above 3.5 and Honors value is above or equal to minimum.
    return Eligible;
}


