#pragma once
#include <map>
#include <string>
#include <set>

int GetPointTotalForStudent(std::map<std::string, std::string> student_info,
                            std::string category);

int GetPointTotalForStudent(std::map<std::string, std::string> student_info);
int GetTopNHomeworkTotalForStudent(
    std::map<std::string, std::string> student_info, int tally);
int GetNumberOfMissingLabsForStudent(
    std::map<std::string, std::string> student_info);
std::map<std::string, std::map<std::string, std::string>> GetIDToInfoFromCSV(
    std::string filename);
std::map<std::string, double> GetIDToGrade(const
    std::map<std::string, std::map<std::string, std::string>>
        & id_to_student_info);
std::set<std::string> GetStudentsEligibleForHonorsCredit(const 
    std::map<std::string, std::map<std::string, std::string>>
        & id_to_student_info,
    const int & minimum);