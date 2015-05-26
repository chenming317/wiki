#include <map>
#include <string>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <bitset>

using namespace std;

class comp {
public:
 static map<string, string> comp_map;
 comp(string s);
 string getComp(){ return compStr;};
 static void init_comp_map();
private:
 string compStr;
};

class dest {
public:
 static map<string, string> dest_map;
 dest(string s);
 string getDest(){ return destStr;};
 static void init_dest_map();
private:
 string destStr;
};

class jump {
public:
 static map<string, string> jump_map;
 jump(string s);
 string getJump(){ return jumpStr;};
 static void init_jump_map();
private:
 string jumpStr;
};

class AInstruction {
public:
 static int varibleAddr;
 static string validChar;
 static string validNum;
 AInstruction(string s);
 string getA(){return addr;};
private:
 string addr;
 bool addrValid();
 string stringtobinary(string s);
 string inttobinary(int n);
};

class CInstruction {
public:
 CInstruction(string s);
 string getC(){return con;};
private:
 string con;
};

class Instruction {
public:
 static map<string, int> symbol_map;
 Instruction(string s);
 string getCode(){return code;};
 static void init_symbol_map();
private:
 string code;
};

map<string, string> comp::comp_map;
map<string, string> dest::dest_map;
map<string, string> jump::jump_map;
map<string, int> Instruction::symbol_map;



string AInstruction::validChar = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_.$";
string AInstruction::validNum = "1234567890";
int AInstruction::varibleAddr = 16;

string AInstruction::stringtobinary(string s)
{
 int addr;
 bitset<16> bitvec;
 ostringstream ostream;
 istringstream istream(s);

 istream >> addr;
 bitvec = addr;
 ostream << bitvec;
 return ostream.str();
}

string AInstruction::inttobinary(int n)
{
 bitset<16> bitvec(n);
 ostringstream ostream;
 ostream << bitvec;
 return ostream.str();
}

AInstruction::AInstruction(string s)
{
 // maybe look up the table to replace the value or symbol
 string::size_type invalid = 0;
 invalid=s.find_first_not_of(validChar);
 if (invalid == string::npos)  // This is a valid address
 {
  // then we check the address is a num , a varible or a symbol
  invalid	= s.find_first_not_of(validNum);
  if (invalid == string::npos) // the address is a num
  {
   // Now we just convert this num to binary code
   addr=stringtobinary(s);
   // cout << addr << endl;
  }
  else // the address is a varible or a symbol
  {
   
   if (Instruction::symbol_map.count(s)) // if the varible or symbol is already exist in the symbol table, just get the value
    addr = inttobinary(Instruction::symbol_map[s]);
   else // This is a varible, in fact, after the first pass, all symbol value must be add into the symbol table
   {
    addr = inttobinary(varibleAddr);
    Instruction::symbol_map.insert(make_pair(s, varibleAddr++));
   }
  }

 }
 else // find invalid char, this is a invalid address, print the error line
 {
      throw runtime_error("Invalid varible name : " + s);
      // cout error with the error line info
 }
}


CInstruction::CInstruction(string s)
{
 string d, c, j;
 string::size_type equal = 0, common = 0;
 equal = s.find("=");
 common = s.find(";");
 if (equal == string::npos && common == string::npos)
  throw runtime_error("ERROR C instruction : " + s);
 if (equal == string::npos)  // dest omit
 {
  d = "";
  c = s.substr(0, common);
  j = s.substr(common+1);
 }
 else if (common == string::npos) // jump omit
 {
  d = s.substr(0, equal);
  c = s.substr(equal+1);
  j = "";
 }
 else // no omit
 {
  d = s.substr(0, equal);
  c = s.substr(equal+1, common - equal -1);
  j = s.substr(common+1);
 }
 dest dest(d);
 comp comp(c);
 jump jump(j);
 con = "111" + comp.getComp() + dest.getDest() + jump.getJump();
}

void Instruction::init_symbol_map()
{
 symbol_map.insert(make_pair("SP",	 0));
 symbol_map.insert(make_pair("LCL",	 1));
 symbol_map.insert(make_pair("ARG",	 2));
 symbol_map.insert(make_pair("THIS",	 3));
 symbol_map.insert(make_pair("THAT",	 4));
 symbol_map.insert(make_pair("R0",	 0));
 symbol_map.insert(make_pair("R1",	 1));
 symbol_map.insert(make_pair("R2",	 2));
 symbol_map.insert(make_pair("R3",	 3));
 symbol_map.insert(make_pair("R4",	 4));
 symbol_map.insert(make_pair("R5",	 5));
 symbol_map.insert(make_pair("R6",	 6));
 symbol_map.insert(make_pair("R7",	 7));
 symbol_map.insert(make_pair("R8",	 8));
 symbol_map.insert(make_pair("R9",	 9));
 symbol_map.insert(make_pair("R10",	 10));
 symbol_map.insert(make_pair("R11",	 11));
 symbol_map.insert(make_pair("R12",	 12));
 symbol_map.insert(make_pair("R13",	 13));
 symbol_map.insert(make_pair("R14",	 14));
 symbol_map.insert(make_pair("R15",	 15));
 symbol_map.insert(make_pair("SCREEN",	 16384));
 symbol_map.insert(make_pair("KBD",	 24576));
}

Instruction::Instruction(string s)
{
 if (s[0] == '@' && s.size() >= 2)
 {
  AInstruction ains(s.substr(1));
  code = ains.getA();
 }
 else
 {
  CInstruction cins(s);
  code = cins.getC();
 }
}

void comp::init_comp_map()
{
 comp_map.insert(make_pair("0",	 "0101010"));
 comp_map.insert(make_pair("1",	 "0111111"));
 comp_map.insert(make_pair("-1",	 "0111010"));
 comp_map.insert(make_pair("D",	 "0001100"));
 comp_map.insert(make_pair("A",	 "0110000"));
 comp_map.insert(make_pair("!D",	 "0001101"));
 comp_map.insert(make_pair("!A",	 "0110001"));
 comp_map.insert(make_pair("-D",	 "0001111"));
 comp_map.insert(make_pair("-A",	 "0110011"));
 comp_map.insert(make_pair("D+1",	"0011111"));
 comp_map.insert(make_pair("A+1",	"0110111"));
 comp_map.insert(make_pair("D-1",	"0001110"));
 comp_map.insert(make_pair("A-1",	"0110010"));
 comp_map.insert(make_pair("D+A",	"0000010"));
 comp_map.insert(make_pair("D-A",	"0010011"));
 comp_map.insert(make_pair("A-D",	"0000111"));
 comp_map.insert(make_pair("D&A",	"0000000"));
 comp_map.insert(make_pair("D|A",	"0010101"));
 comp_map.insert(make_pair("M",	 "1110000"));
 comp_map.insert(make_pair("!M",	 "1110001"));
 comp_map.insert(make_pair("-M",	 "1110011"));
 comp_map.insert(make_pair("M+1",	"1110111"));
 comp_map.insert(make_pair("M-1",	"1110010"));
 comp_map.insert(make_pair("D+M",	"1000010"));
 comp_map.insert(make_pair("D-M",	"1010011"));
 comp_map.insert(make_pair("M-D",	"1000111"));
 comp_map.insert(make_pair("D&M",	"1000000"));
 comp_map.insert(make_pair("D|M",	"1010101"));
}

comp::comp(string s)
{
 if (comp_map.count(s))
  compStr = comp_map[s];
}

void dest::init_dest_map()
{
 dest_map.insert(make_pair("",	 "000"));
 dest_map.insert(make_pair("M",	 "001"));
 dest_map.insert(make_pair("D",	 "010"));
 dest_map.insert(make_pair("MD",	 "011"));
 dest_map.insert(make_pair("A",	 "100"));
 dest_map.insert(make_pair("AM",	 "101"));
 dest_map.insert(make_pair("AD",	 "110"));
 dest_map.insert(make_pair("AMD",	"111"));
}

dest::dest(string s)
{
 if (dest_map.count(s))
  destStr = dest_map[s];
}

void jump::init_jump_map()
{
 jump_map.insert(make_pair("",	 "000"));
 jump_map.insert(make_pair("JGT",	"001"));
 jump_map.insert(make_pair("JEQ",	"010"));
 jump_map.insert(make_pair("JGE",	"011"));
 jump_map.insert(make_pair("JLT",	"100"));
 jump_map.insert(make_pair("JNE",	"101"));
 jump_map.insert(make_pair("JLE",	"110"));
 jump_map.insert(make_pair("JMP",	"111"));
}

jump::jump(string s)
{
 if (jump_map.count(s))
  jumpStr = jump_map[s];
}


// opens in binding it to the given file
ifstream& open_file(ifstream &in, const string &file)
{
 in.close();
 // close in case it was already open
 in.clear();
 // clear any existing errors
 // if the open fails, the stream will be in an invalid state
 in.open(file.c_str()); // open the file we were given
 return in; // condition state is good if open succeeded
}

int main(int argc, char **argv)
{
 if (argc != 3)
  throw runtime_error("wrong number of arguments");

 ifstream input;
 if (!open_file(input, argv[1]))
  throw runtime_error("no input file");

 string line;
 string whitespace = " \t\r";
 string::size_type startLine = 0, endLine =0;
 string comments="//";
 string validLine , tempLine;

 ofstream output;
 output.open(argv[2]);
 if (!output)
  throw runtime_error("no output file");

 ofstream outtemp;
 outtemp.open("temp.hack");
 if (!outtemp)
  throw runtime_error("create temp file failed!");

 comp::init_comp_map();
 dest::init_dest_map();
 jump::init_jump_map();
 Instruction::init_symbol_map();

 /*This is the first pass, we mainly do the following jobs:
 * 1: Extract the valid command line, not account for empty line or a whole comment line, and output the result to a temp file.
 * 2: If there there is a inline comments, we just ignore the comments.
 * 3. If there are some varibles or symbols, on the current stage, we just handle the symbols by adding them to the symbol table.
 */
 int validLineNum = 0, lineNum = 0;
 map<string, int>::const_iterator map_it;
 while (getline(input, line)) {
  lineNum++;
  //cout << line << endl;
  startLine = line.find_first_not_of(whitespace);
  endLine	= line.find(comments);
 
  if (startLine == string::npos)
  {
   cout << "empty line" << endl;
   continue;
  }
  // Now not a empty line
  if (endLine != string::npos) // find comment
  {
   if (startLine == endLine) // Whole line is comment
   {
    cout << "Whole line is comment" << endl; continue;
   }
   tempLine = line.erase(endLine, line.size() - endLine);
   endLine = tempLine.find_last_not_of(whitespace) + 1;
  }
  else  // cannot find comment
  {
   endLine = line.find_last_not_of(whitespace) + 1;
  }
   
  validLine = line.substr(startLine, endLine - startLine);

  /*check if this valid line is a symbol or not*/
  if (validLine.size() >=3 && validLine[0] == '(' && validLine[validLine.size() - 1] == ')') // maybe a symbol
  {
   string::size_type invalid = 0;
   string symbol = validLine.substr(1, validLine.size() - 2);
   invalid=symbol.find_first_not_of(AInstruction::validChar);
   if (invalid == string::npos) // this is a valid sybmol, add it to the symbol table
   {
    Instruction::symbol_map.insert(make_pair(symbol,validLineNum));
    continue;
   
   }
   else // This is a invalid symbol, print error message then exit
   {
    throw runtime_error("ERROR -- A invalid symbol exist! LINE = " + lineNum);
   }
  }

  /*Not a symbol*/
  validLineNum++;
  //cout << "startLine =" << startLine << endl;
  //cout << "endLine =" << endLine << endl;
  //cout << "validLine =" << validLine << endl;
 
  outtemp << validLine << endl;
 
  //Instruction ins(validLine);
  //outtemp << ins.getCode() << endl ;
  //cout << "code : " << ins.getCode() << endl << endl;
 }
 



 /*Second pass*/
 input.close();
 input.clear();
 input.open("temp.hack");
 while (getline(input, line)) {
  Instruction ins(line);
  output << ins.getCode() << endl ;
 }

 /*
 map_it = Instruction::symbol_map.begin();
 while (map_it != Instruction::symbol_map.end()) {
  cout << map_it->first << "\t" << map_it->second << endl;
  ++map_it;
 }
 */
 
 return 0;
}

