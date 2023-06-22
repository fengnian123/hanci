#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include<map>
#include<set>
#include <unordered_map>
#include<time.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

class QsoRecord {// Define the fields in each record
public:
    string qso_date;
    string time_on;
    string call;
    string other;
};

// A hash table to store QSO records
map<string, QsoRecord> records;

//Converts the case of the record
string convert(string temp){
    string re=temp;
    int judge=0,begin=0;
    for(int i=0;i<temp.size();i++){
        //Converts field names in <> to uppercase letters
        if(temp[i]=='<'||begin==1){
            judge=1;
            begin=1;
        }
        if(temp[i]=='>'){
            judge=0;
            begin=0;
        }
        if(temp[i]>=97&&temp[i]<=122&&judge==1){
            re[i]=temp[i]-32;
        }
        else{//The field content is not transformed
            re[i]=temp[i];
        }
    }
    return re;
}
int ct_records=0;

// Imports data from a CSV or ADIF file into the database
void import_file(const string& filename) {
    QsoRecord input[10000];
    string all;
    int ct_input=0;
    int judge=-1;//Determine the format of the input file
    if(filename.size() >= 4 && filename.substr(filename.size()-4) == ".csv"){
        judge = 0;
    }
    else if(filename.size() >= 4 && filename.substr(filename.size()-4) == ".adi"){
        judge = 1;
    }
    ifstream file_cin;
    file_cin.open(filename);//Open the file to import
    string temp;
    ofstream file_cout;
    int find_eoh=0;
    int line=1;
    if(judge==1){//.adi file import
        while (getline(file_cin, temp)) {
            // Parse fields based on file format
            string upper_temp=convert(temp);
            all.append(upper_temp);//Compress all records to one line
        } 
        if(all.find("<EOH>")!=-1){//Delete all headers before <EOH>
            all.substr(all.find("<EOH>")+5);
        }
        file_cout.open("temp_database.bin",ios::app | ios::binary);
        int position=0;
        while(all.find("<QSO_DATE:8>")!=-1){
            QsoRecord record;
            all=all.substr(all.find("<QSO_DATE:8>")+12);
            string key=all.substr(0,8);
            record.qso_date=key;//Find the primary key of the record
            all=all.substr(all.find("<TIME_ON:6>")+11);
            key=all.substr(0,all.find("<"));//Find the primary key of the record
            int size=0,i=0;
            while(key[i++]!=' '){
                size++;
            }
            while(size<6){//Extended time primary key
                key.replace(size,1,"");
                key=key+"0";
                size++;
            }
            record.time_on=key;
            record.other=all.substr(all.find("<"),all.find("<QSO_DATE:8>")-all.find("<"));
            file_cout<<record.qso_date<<record.time_on<<record.other<<endl;//Write a record
        }
    }
    if(judge==0){//.csv file import
        file_cout.open("temp_database.bin",ios::app | ios::binary);
        int line=1;
        string key[10000];//Record all field names
        int key_ct=0;
        while (getline(file_cin, temp)) {
            QsoRecord record;
            // Parse fields based on file format
            string upper_temp=convert(temp);
            if(line==1){//Look for the field name in the first line
                line++;
                while(temp.find(",")!=-1){
                    key[key_ct++]=temp.substr(0,temp.find(","));
                    temp=temp.substr(temp.find(",")+1);
                }
                key[key_ct++]=temp;
            }
            else{
                record.qso_date=temp.substr(0,8);//Find the primary key qso_date of the record
                temp=temp.substr(9);
                int size=0,i=0;
                string key_time=temp.substr(0,temp.find(","));//Find the primary key time_on of the record
                temp=temp.substr(temp.find(","));
                while(key_time[i++]!=' '){
                    size++;
                }
                while(size<6){//Extended time primary key
                    key_time.replace(size,1,"");
                    key_time=key_time+"0";
                    size++;
                }
                record.time_on=key_time;
                file_cout<<record.qso_date<<record.time_on;//Write the primary key of a record
                string contain;
                key_ct=2;
                int temp_size=temp.size();
                string ttemp=temp;
                for(int i=0;i<temp_size;i++){
                    if(temp[i]=='"'){//Handles cases with "," in the field
                        contain=temp.substr(i+1,temp.find("\"",i+1)-i-1);
                        file_cout<<"<"<<key[key_ct++]<<">"<<"\""<<contain<<"\"";
                        i=temp.find("\"",i+1);
                    }
                    else if(temp[i]==','){//find each field content
                        i++;
                        while(temp[i]==' '&&i<temp_size){
                            i++;
                        }
                        if(temp[i]=='"'){
                            i--;
                            continue;
                        }
                        i--;
                        if(temp.find(",",i+1)!=-1){
                            contain=temp.substr(i+1,temp.find(",",i+1)-i-1);
                            file_cout<<"<"<<key[key_ct++]<<">"<<contain;
                        }
                        else{
                            contain=temp.substr(i+1);
                            file_cout<<"<"<<key[key_ct++]<<">"<<contain;
                        }
                    }
                }
                file_cout<<endl;
            }
        }
    }
    file_cin.close();
    file_cin.open("temp_database.bin");
    while (getline(file_cin, temp)){//Open the temporary database
        QsoRecord record;
        record.qso_date=temp.substr(0,8);
        record.time_on=temp.substr(8,6);
        record.other=temp.substr(14);//Overwrite the newly imported content
        string primary_key=record.qso_date+record.time_on;
        records[primary_key]=record;
    }
    file_cin.close();
    file_cout.close();
    file_cout.open("database.bin");
    for (auto it = records.rbegin(); it != records.rend(); ++it) {//Write all records to the target database
        file_cout<<it->second.qso_date<<it->second.time_on<<it->second.other<<endl;
    }
    file_cin.close();
}

//Exports the database to a CSV or ADIF file
void export_file(const string& filename) {
    int judge=-1;
    int time=1;
    //Determine the file type by filename extension
    if(filename.size() >= 4 && filename.substr(filename.size()-4) == ".csv"){
        judge = 0;
    }
    else if(filename.size() >= 4 && filename.substr(filename.size()-4) == ".adi"){
        judge = 1;
    }
    ifstream file_cin;
    file_cin.open("database.bin");//Open the database
    ofstream file_cout;
    file_cout.open(filename);
    string temp;
    if(judge==0){//export .csv file
        map<string,string> word;
        while(getline(file_cin,temp)){
            while(temp.find("<")!=-1){//find all fields
                temp=temp.substr(temp.find("<")+1);
                string ttemp=temp.substr(0,temp.find(">"));
                string ttemp_contain=temp.substr(temp.find(">")+1,temp.find("<")-temp.find(">")-1);
                while(ttemp.find(":")!=-1){
                    ttemp=ttemp.substr(0,ttemp.find(":"));
                }
                if(ttemp.substr(0,3)!="EOR"){//ignore the <EOR> field
                    word[ttemp]=ttemp_contain;
                }
                temp=temp.substr(temp.find(">")+1);
            }
        }
        file_cin.close();
        file_cin.open("database.bin");
        file_cout<<"QSO_DATE,TIME_ON";
        for (auto it = word.begin(); it != word.end(); ++it) {
                file_cout<<","<<it->first;//write all field of records
            }
        file_cout<<endl;
        while(getline(file_cin,temp)){//find all field contains in a record
            file_cout<<temp.substr(0,8)<<","<<temp.substr(8,6)<<",";//write the primary key
            map<string,string> word_line;
            while(temp.find("<")!=-1){
                temp=temp.substr(temp.find("<")+1);
                string ttemp=temp.substr(0,temp.find(">"));
                string ttemp_contain=temp.substr(temp.find(">")+1,temp.find("<")-temp.find(">")-1);
                while(ttemp.find(":")!=-1){//get the real contain of a field
                    ttemp=ttemp.substr(0,ttemp.find(":"));
                }
                if(ttemp.substr(0,3)!="EOR"){//ignore the <EOR> field
                    word_line[ttemp]=ttemp_contain;
                }
                temp=temp.substr(temp.find(">")+1);
            }
            for (auto it = word.begin(); it != word.end(); ++it) {//write all field contains of a record
                auto itt = word_line.find(it->first);
                if(itt!=word_line.end()&&it == word.begin()){
                    file_cout<<itt->second;
                }
                else if(itt!=word_line.end()&&it != word.begin()){//handle the last ","
                    file_cout<<","<<itt->second;
                }
                else if(itt==word_line.end()&&it != word.begin()){
                    file_cout<<",";
                }
            }
            file_cout<<endl;
        }
    }
    else if(judge==1){//export .adi file
        while(getline(file_cin,temp)){
            file_cout<<"<QSO_DATE:8>"<<temp.substr(0,8)<<"<TIME_ON:6>"<<temp.substr(8,6);//write the primary key
            map<string,string> word;
            while(temp.find("<")!=-1){//find all field and contains in a record
                temp=temp.substr(temp.find("<")+1);
                string ttemp=temp.substr(0,temp.find(">"));
                string ttemp_contain=temp.substr(temp.find(">")+1,temp.find("<")-temp.find(">")-1);
                while(ttemp.find(":")!=-1){//get the real contain of a field
                    ttemp=ttemp.substr(0,ttemp.find(":"));
                }
                if(ttemp.substr(0,3)!="EOR"){//ignore the <EOR> field
                    word[ttemp]=ttemp_contain;
                }
                temp=temp.substr(temp.find(">")+1);
            }
            for (auto it = word.begin(); it != word.end(); ++it) {
                int count=0;
                for(int i=0;i<it->second.size();i++){
                    if(it->second[i]!=' '&&it->second[i]!='\"'){
                        count++;//record the size of the field contain
                    }
                }
                file_cout<<"<"<<it->first<<":"<<count<<">"<<it->second;//write the record
            }
            file_cout<<"<EOR>"<<endl;
        }
    }
}

// Searches for records with the exact same call field
void search_records(string& call) {
    ifstream file_cin;
    ofstream file_cout;
    file_cin.open("database.bin");//open the database
    string temp;
    int times=0;
    while(getline(file_cin,temp)){//find each <call> field of records
        string ttemp=temp.substr(temp.find("<CALL")+5);
        ttemp=ttemp.substr(ttemp.find(">")+1,ttemp.find("<")-ttemp.find(">")-1);
        int final=ttemp.size()-1;
        while(ttemp[final]==' '){//delete the space of field contains
            ttemp.replace(final,1,"");
            final--;
        }
        final=call.size()-1;
        while(call[final]==' '){//delete the space of the string to search
            call.replace(final,1,"");
            final--;
        }
        while(ttemp[final]==' '){
            ttemp.replace(final,1,"");
            final--;
        }
        int i=0,j=0;
        int judge=1;
        while(i<ttemp.size()&&j<call.size()){//judge whether the strings are the same
            if(ttemp[i]==' '){//ignore the space
                i++;
                continue;
            }
            if(ttemp[j]==' '){//ignore the space
                j++;
                continue;
            }
            if(ttemp[i++]!=call[j++]){
                judge=0;
            }
        }
        if(i!=ttemp.size()||j!=call.size()){
            judge=0;
        }
        if(judge==1){//if find the same <call>,output the record
            cout<<"QSO_DATE:"<<temp.substr(0,8)<<",TIME_ON:";
            int begin=14;
            while(begin<temp.size()){
                if(temp.find("<EOR>")!=-1){//delete the <EOR> field
                    temp.replace(temp.find("<EOR>"),5,"");
                }
                if(temp[begin]=='<'){//Reformat the record
                    string ttemp=temp.substr(begin+1,temp.find(">",begin+1)-begin-1);
                    while(ttemp.find(":")!=-1){
                        ttemp=temp.substr(begin+1,temp.find(":",begin+1)-begin-1);
                    }
                    temp.replace(begin,temp.find(">",begin+1)-begin+1,","+ttemp+":");
                    begin+=ttemp.size();
                    continue;
                }
                begin++;
            }
            cout<<temp.substr(8)<<endl;//output the final record
            cout<<endl;
            times++;//the number of records found increases
        }
    }
    cout<<times<<" record(s) found"<<endl;
}

// Outputs all records within the closed interval of the specified time range
void output_records_in_time_range(const string& start_datetime, const string& end_datetime) {
    ifstream file_cin;
    ofstream file_cout;
    file_cin.open("database.bin");//open the database
    string temp;
    int times=0;
    while(getline(file_cin,temp)){//find each primary key of records
        //determine whether the time of record is within the range of input time interval
        if(temp.substr(0,14)>=start_datetime&&temp.substr(0,14)<=end_datetime){
            cout<<"QSO_DATE:"<<temp.substr(0,8)<<",TIME_ON:";
            int begin=14;
            while(begin<temp.size()){
                if(temp.find("<EOR>")!=-1){//ignore the <EOR> field
                    temp.replace(temp.find("<EOR>"),5,"");
                }
                if(temp[begin]=='<'){//find each field and contains of the record
                    string ttemp=temp.substr(begin+1,temp.find(">",begin+1)-begin-1);
                    while(ttemp.find(":")!=-1){
                        ttemp=temp.substr(begin+1,temp.find(":",begin+1)-begin-1);
                    }
                    temp.replace(begin,temp.find(">",begin+1)-begin+1,","+ttemp+":");
                    begin+=ttemp.size();
                    continue;
                }
                begin++;
            }
            cout<<temp.substr(8)<<endl;//output the record
            cout<<endl;
            times++;//the number of records found increases
        }
    }
    cout<<times<<" record(s) found"<<endl;
}

// Deletes records with primary keys that exist in the import file
void delete_records(const string& filename) {
    int judge=-1;
    int time=1;
    //Determine the format of the input file
    if(filename.size() >= 4 && filename.substr(filename.size()-4) == ".csv"){
        judge = 0;
    }
    else if(filename.size() >= 4 && filename.substr(filename.size()-4) == ".adi"){
        judge = 1;
    }
    ifstream file_cin1;
    ifstream file_cin2;
    ofstream file_cout;
    file_cin2.open(filename);//open the file to delete
    string temp1,temp2;
    int line=1;
    string all;
    set<string> delete_keys;//record the key to delete
    if(judge==0){//.csv case
        while(getline(file_cin2,temp2)){//traversal the file to delete
            if(judge==0){
                temp2=convert(temp2);
                string delete_key;
                int delete_ct=0,dou_time=0;
                if(line==1){//ignore the first line
                    line++;
                    continue;
                }
                for(int i=0;i<temp2.size();i++){//get the key to delete
                    if(temp2[i]>='0'&&temp2[i]<='9'){
                        delete_key+=temp2[i];
                    }
                    else if(temp2[i]==','){
                        dou_time++;
                        if(dou_time==2){//record the key until the secord ","
                            break;
                        }
                    }
                }
                if(delete_key.size()<14){
                    delete_key.append("0");
                }
                delete_keys.insert(delete_key);//insert the key to delete to the set
            }
        }
    }
    else if(judge==1){//.adi case
        if(judge==1){
            while(getline(file_cin2,temp2)) {
                // Parse fields based on file format
                temp2=convert(temp2);
                all.append(temp2);
            }
            if(all.find("<EOH>")!=-1){//conpress all records to one string
                all.substr(all.find("<EOH>")+5);//delete <EOR> field
            }
            while(all.find("<QSO_DATE:8>")!=-1){//find all keys to delete 
                string delete_key;
                all=all.substr(all.find("<QSO_DATE:8>")+12);
                delete_key+=all.substr(0,8);
                all=all.substr(all.find("<TIME_ON:6>")+11);
                delete_key+=all.substr(0,all.find("<"));
                int size=0,position=0;
                while(delete_key[position++]!=' '){
                    size++;
                }
                while(size<14){//add "00" if the time is 4 bits
                    delete_key.replace(size,1,"");
                    delete_key=delete_key+"0";
                    size++;
                }
                while(delete_key.size()>14){//delete extra space
                    delete_key.pop_back();
                }
                delete_keys.insert(delete_key);//insert the key to the set
            }
        }
    }
    file_cin2.close();
    file_cin1.open("database.bin");
    file_cout.open("temp_database.bin");
    file_cout.close();//clear the temp_database.bin file
    file_cout.open("temp_database.bin",ios::app | ios::binary);
    while(getline(file_cin1,temp1)){
            //look for each record in the keys to delete
            auto iter = delete_keys.find(temp1.substr(0,14));
            if (iter != delete_keys.end()) {//if it is in them,doon't write it to the temp_database.bin file
                continue;
            } 
            else {
                file_cout<<temp1<<endl;
            }
        }
    file_cin1.close();
    file_cout.close();
    file_cin1.open("temp_database.bin");
    file_cout.open("database.bin");
    file_cout.close();//clear the temp_database.bin file
    file_cout.open("database.bin",ios::app | ios::binary);
    while(getline(file_cin1,temp1)){
        file_cout<<temp1<<endl;//write the database.bin from temp_database.bin
    }
    file_cin1.close();
}

int main(int argc, char* argv[]) {
    //system("test.bat");
    //Read command line arguments
    if (argc < 3) {
        cout << "Usage: qso_mgr <action> <filename>" << endl;
        return 1;
    }
    char action = argv[1][1];//get the command type
    string filename=argv[2];
    if(action=='i'){//-i command
        import_file(filename);
    }
    else if(action=='o'){//-o command
        export_file(filename);
    }
    else if(action=='s'){//-sentry command
        search_records(filename);
    }
    else if(action=='l'){//-l command
        if (argc < 4) {
            cout << "Usage: qso_mgr l <start datetime> <end datetime>" << endl;
            return 1;
        }
        string start_datetime=argv[2], end_datetime=argv[3];//get the time interval
        output_records_in_time_range(start_datetime, end_datetime);
    }
    else if(action=='d'){//-d command
        delete_records(filename);
    }
    else{
        cout<<"Invalid Command"<<endl;
        return 1;
    }
    return 0;
}

