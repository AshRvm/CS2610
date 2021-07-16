#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

/*
TEAM MEMBERS :
    cs19b007 - Aswin Ramesh	
    cs18b053 - B Sai Chaitanya
    cs18b022 - K V Pradeep Reddy
*/

string HexToBinary(string hex){
    string bin = "";
    for(int i=0;i<(int)hex.size();i++){
        if(hex[i] == '0') bin += "0000";
        else if(hex[i] == '1') bin += "0001";
        else if(hex[i] == '2') bin += "0010";
        else if(hex[i] == '3') bin += "0011";
        else if(hex[i] == '4') bin += "0100";
        else if(hex[i] == '5') bin += "0101";
        else if(hex[i] == '6') bin += "0110";
        else if(hex[i] == '7') bin += "0111";
        else if(hex[i] == '8') bin += "1000";
        else if(hex[i] == '9') bin += "1001";
        else if(hex[i] == 'a') bin += "1010";
        else if(hex[i] == 'b') bin += "1011";
        else if(hex[i] == 'c') bin += "1100";
        else if(hex[i] == 'd') bin += "1101";
        else if(hex[i] == 'e') bin += "1110";
        else if(hex[i] == 'f') bin += "1111";
    }
    return (bin)
}

string BinaryToHex(string bin){
    string hex = "";
    for(int i=0;i<(int)bin.size()/4;i++){
        string temp = "";
        for(int j=0;j<4;j++) temp+= bin[4*i+j];
        if(temp == "0000") hex += "0";
        else if(temp == "0001") hex += "1";
        else if(temp == "0010") hex += "2";
        else if(temp == "0011") hex += "3";
        else if(temp == "0100") hex += "4";
        else if(temp == "0101") hex += "5";
        else if(temp == "0110") hex += "6";
        else if(temp == "0111") hex += "7";
        else if(temp == "1000") hex += "8";
        else if(temp == "1001") hex += "9";
        else if(temp == "1010") hex += "a";
        else if(temp == "1011") hex += "b";
        else if(temp == "1100") hex += "c";
        else if(temp == "1101") hex += "d";
        else if(temp == "1110") hex += "e";
        else if(temp == "1111") hex += "f";
    }
    return (hex);
}

int BinaryToInt(string bin){
    int result=0;
    for(int i=0;i<(int)bin.size();i++){
        result = result*2 + (int)bin[i] - 48;
    }
    return (result);
}

int BinaryCompToInt(string bin){
    int result=0;
    int negate=-1;
    for(int i=1;i<(int)bin.size();i++){
        result = result*2 + (int)bin[i] - 48;
        negate *= 2;
    }
    negate = ((int)bin[0] - 48)*negate;
    return (negate+result);
}

string IntToBinary(int x, int size){
    string bin = "";
    string temp = "";
    while(x > 0){
        if(x%2 == 0) temp += '0';
        else temp += '1';
        x /= 2;
    }
    int tempSize = (int)temp.size();
    for(int i=0;i<tempSize;i++){
        bin += temp[tempSize-i-1];
    }
    if(size == 0) return (bin);
    int sizeDiff = size - (int)temp.size();
    for(int i=0;i<sizeDiff;i++) bin = '0' + bin;
    return (bin);
}

string IntToBinaryComp(int x){
    if(x >= 0) return(IntToBinary(x,8));
    else{
        string temp = IntToBinary(-x,0);
        int tempsize = (int)temp.size();
        for(int i=0;i<tempsize;i++){
            if(temp[i] == '0') temp[i] = '1';
            else temp[i] = '0';
        }
        int index = tempsize-1;
        while((temp[index] != '0') && (index >= 0)){
            temp[index] = '0';
            index--;
        }
        temp[index] = '1';
        for(int i=0;i<8-tempsize;i++) temp = "1" + temp;
        return temp;
    }
}

class Cache{
    friend class Pipeline;
    vector<string> dCache;
    vector<string> iCache;
    public:
        void getData(vector<string> &data){
            dCache = data;
        }
        void getInstructions(vector<string> &inst){
            iCache = inst;
        }
};

class RegisterFile{
    friend class Pipeline;
    vector<string> registers;
    vector<bool> status;
    public:
        RegisterFile(){
            for(int i=0;i<16;i++) status.push_back(false);
        }
        void getRegisters(vector<string> &reg){
            registers = reg;
        }
};

class Pipeline{
    private:
        bool haltCalled = false;
        bool halted = false;

        bool stallIF = false;
        bool stallID = false;
        bool stallEX = false;
        bool stallMEM = false;
        bool stallWB = false;

        bool runIF = true;
        int IF_PC = 0;
        void IF();

        bool runID = false;
        int ID_PC = 0;
        string ID_IR = "";
        void ID();

        bool runEX = false;
        int EX_PC = 0;
        string EX_IR = "";
        string regA = "";
        string regB = "";
        void EX();

        bool runMEM = false;
        bool condMEM = false;
        string MEM_IR = "";
        string MEM_ALUOutput = "";
        void MEM();

        bool runWB = false;
        bool condWB = false;
        string WB_IR = "";
        string WB_ALUOutput = "";
        string LMD = "";
        void WB();

        void cycle();
        void flush();

        int clockCycle;
        int numInstruction;
        int numArithmeticInstructions;
        int numLogicalInstructions;
        int numDataInstructions;
        int numControlInstructions;
        int numHaltInstructions;
        int totalStalls;
        int dataStalls;
        int controlStalls;
        Cache* C;
        RegisterFile* R;
    public:
        Pipeline(){
            clockCycle = 0;
            numInstruction = 0;
            numArithmeticInstructions = 0;
            numLogicalInstructions = 0;
            numDataInstructions = 0;
            numControlInstructions = 0;
            numHaltInstructions = 0;
            totalStalls = 0;
            dataStalls = 0;
            controlStalls = 0;
        }
        void getCache(Cache* tempC){
            C = tempC;
        }
        void getRegisterFile(RegisterFile* tempR){
            R = tempR;
        }
        void run(){
            while(!haltCalled && !halted){
                cycle();
            }
            while(haltCalled){
                if(runEX == false && runMEM == false && runWB == false){
                    halted = true;
                    haltCalled = false;
                    numInstruction = numArithmeticInstructions + numLogicalInstructions + numDataInstructions + numControlInstructions + numHaltInstructions;
                    return ;
                }
                cycle();
            }
        }
        void printOutput(){
            ofstream outFile;
            outFile.open("Output.txt");

            outFile << "Total number of instructions executed: " <<numInstruction<<endl;
            outFile << "Number of instructions in each class" <<endl;
            outFile << "Arithmetic instructions              : " <<numArithmeticInstructions<<endl;
            outFile << "Logical instructions                 : " <<numLogicalInstructions<<endl;
            outFile << "Data instructions                    : " <<numDataInstructions<<endl;
            outFile << "Control instructions                 : " <<numControlInstructions<<endl;
            outFile << "Halt instructions                    : " <<numHaltInstructions<<endl;
            outFile << "Cycles Per Instruction               : " <<((double) clockCycle / numInstruction) <<endl;
            outFile << "Total number of stalls               : " <<dataStalls+controlStalls <<endl;
            outFile << "Data stalls (RAW)                    : " <<dataStalls <<endl;
            outFile << "Control stalls                       : " <<controlStalls <<endl;

            outFile.close();

            ofstream cacheFile;
            cacheFile.open("DCache.txt");
            for(int i=0;i<256;i++){
                cacheFile<<C->dCache[i]<<endl;
            }
        }
};

void Pipeline::cycle(){
    clockCycle++;
    if(clockCycle > 25) haltCalled = true;
    // cout<<"CLOCK CYCLE NUM: "<<clockCycle<<endl;
    WB();
    MEM();
    EX();
    ID();
    IF();
}

void Pipeline::IF(){
    // cout<<"PC count : "<<IF_PC<<endl;
    if(!runIF){
        return;
    }
    if(runID || stallID){
        runIF = true;
        return;
    }
    string PC_inst = C->iCache[IF_PC] + C->iCache[IF_PC+1];
    ID_IR = HexToBinary(PC_inst);
    // cout<<PC_inst<<endl;
    IF_PC += 2;
    // cout<<PC_inst<<endl;
    runID = true;
}

void Pipeline::ID(){
    // cout<<"NUM OF CYCLES :"<<clockCycle<<endl;
    if((!runID) || stallID){
        return;
    }
    if(runEX || stallEX){
        runID = true;
        return;
    }
    EX_IR = ID_IR;
    string OPCode = "";
    for(int i=0;i<4;i++) OPCode += EX_IR[i];
    // cout<<"OPCODE : "<<OPCode<<endl;

    //halt
    if(OPCode == "1111"){
        runIF = false;
        runID = false;
        numHaltInstructions ++;
        haltCalled = true;
        return;
    }
    runID = false;
    runEX = true;

    //jmp
    if(OPCode == "1010"){
        stallID = true;
        regA = "";
        for(int i=4;i<12;i++) regA += ID_IR[i];
        EX_PC = IF_PC;
        controlStalls += 2;
        return;
    }

    //beqz  
    if(OPCode == "1011"){
        stallID = true;
        string R1 = "";
        for(int i=4;i<8;i++) R1 += ID_IR[i];
        if(R->status[BinaryToInt(R1)] == true){
            dataStalls++;
            runEX = false;
            runID = true;
            stallID = false;
            return;
        }
        controlStalls += 2;
        EX_PC = IF_PC;
        regA = HexToBinary(R->registers[BinaryToInt(R1)]);
        regB = "";
        for(int i=8;i<16;i++) regB += ID_IR[i];
        return;
    }

    //store
    if(OPCode == "1001"){
        string tempAddr = "";
        for(int i=8;i<12;i++) tempAddr += ID_IR[i];
        if(R->status[BinaryToInt(tempAddr)] == true){
            dataStalls++;
            runEX = false;
            runID = true;
            return;
        }
        regA = HexToBinary(R->registers[BinaryToInt(tempAddr)]);
        regB = "";
        for(int i=12;i<16;i++) regB += ID_IR[i];
        return;
    }

    //load
    if(OPCode == "1000"){
        string tempAddr1 = "";
        string tempAddr2 = "";
        for(int i=4;i<8;i++) tempAddr1 += ID_IR[i];
        for(int i=8;i<12;i++) tempAddr2 += ID_IR[i];
        if(R->status[BinaryToInt(tempAddr2)] == true){
            dataStalls++;
            runEX = false;
            runID = true;
            return;
        }
        regB = "";
        for(int i=12;i<16;i++) regB += ID_IR[i];
        regA = HexToBinary(R->registers[BinaryToInt(tempAddr2)]);
        R->status[BinaryToInt(tempAddr1)] = true;
        return;
    }

    //add, sub, mul
    if((OPCode == "0000") || (OPCode == "0001") || (OPCode == "0010")){
        string tempAddr1 = "";
        string tempAddr2 = "";
        string tempAddr3 = "";
        for(int i=4;i<8;i++) tempAddr1 += ID_IR[i];
        for(int i=8;i<12;i++) tempAddr2 += ID_IR[i];
        for(int i=12;i<16;i++) tempAddr3 += ID_IR[i];
        if(R->status[BinaryToInt(tempAddr2)] == true || R->status[BinaryToInt(tempAddr3)] == true){
            dataStalls++;
            runEX = false;
            runID = true;
            return;
        }
        regA = HexToBinary(R->registers[BinaryToInt(tempAddr2)]);
        regB = HexToBinary(R->registers[BinaryToInt(tempAddr3)]);
        R->status[BinaryToInt(tempAddr1)] = true;
        return;
    }

    //inc
    if(OPCode == "0011"){
        string tempAddr="";
        for(int i=4;i<8;i++) tempAddr += ID_IR[i];
        if(R->status[BinaryToInt(tempAddr)] == true){
            dataStalls++;
            runEX = false;
            runID = true;
            return;
        }
        // cout<<regA<<endl;
        // cout<<R->registers[2]<<endl;
        R->status[BinaryToInt(regA)] = true;
        regA = HexToBinary(R->registers[BinaryToInt(tempAddr)]);
        // cout<<endl;
        // cout<<"VALUE IN INC REG : "<<regA<<" ? "<<endl<<endl; 
        return;
    }

    //and, or, xor
    if((OPCode == "0100") || (OPCode == "0101") || (OPCode == "0111")){
        string tempAddr1 = "";
        string tempAddr2 = "";
        string tempAddr3 = "";
        for(int i=4;i<8;i++) tempAddr1 += ID_IR[i];
        for(int i=8;i<12;i++) tempAddr2 += ID_IR[i];
        for(int i=12;i<16;i++) tempAddr3 += ID_IR[i];
        if(R->status[BinaryToInt(tempAddr2)] == true || R->status[BinaryToInt(tempAddr3)] == true){
            dataStalls++;
            runEX = false;
            runID = true;
            return;
        }
        regA = HexToBinary(R->registers[BinaryToInt(tempAddr2)]);
        regB = HexToBinary(R->registers[BinaryToInt(tempAddr3)]);
        R->status[BinaryToInt(tempAddr1)] = true;
        return;
    }

    //not
    if(OPCode == "0110"){
        string tempAddr1 = "";
        string tempAddr2 = "";
        for(int i=4;i<8;i++) tempAddr1 += ID_IR[i];
        for(int i=8;i<12;i++) tempAddr2 += ID_IR[i];
        if(R->status[BinaryToInt(tempAddr2)] == true){
            dataStalls++;
            runEX = false;
            runID = true;
            return;
        }
        regA = HexToBinary(R->registers[BinaryToInt(tempAddr2)]);
        R->status[BinaryToInt(tempAddr1)] = true;
    }
}

void Pipeline::EX(){
    if(!runEX || stallEX) return;
    string OPCode = "";
    for(int i=0;i<4;i++) OPCode += EX_IR[i];

    if(OPCode == "0000"){
        int result = BinaryCompToInt(regA) + BinaryCompToInt(regB);
        MEM_ALUOutput = IntToBinaryComp(result);
        numArithmeticInstructions++;
    }
    else if(OPCode == "0001"){
        int result = BinaryCompToInt(regA) - BinaryCompToInt(regB);
        MEM_ALUOutput = IntToBinaryComp(result);
        // cout<<"CHECK FOR SUB :"<<result<<endl;
        // cout<<"CHECK BIN OF SUB :"<<MEM_ALUOutput<<endl;
        numArithmeticInstructions++;
    }
    else if(OPCode == "0010"){
        int result = BinaryCompToInt(regA) * BinaryCompToInt(regB);
        MEM_ALUOutput = IntToBinaryComp(result);
        numArithmeticInstructions++;
    }
    else if(OPCode == "0011"){
        int result = BinaryCompToInt(regA) + 1;
        // cout<<"INCREMENTED VALUE : "<<result<<endl<<endl;
        MEM_ALUOutput = IntToBinaryComp(result);
        numArithmeticInstructions++;
    }
    else if(OPCode == "0100"){
        MEM_ALUOutput = "";
        for(int i=0;i<8;i++){
            if((regA[i] == '1') && (regB[i] == '1')) MEM_ALUOutput += '1';
            else MEM_ALUOutput += '0';
        }
        numLogicalInstructions++;
    }
    else if(OPCode == "0101"){
        MEM_ALUOutput = "";
        for(int i=0;i<8;i++){
            if((regA[i] == '1') || (regB[i] == '1')) MEM_ALUOutput += '1';
            else MEM_ALUOutput += '0';
        }
        numLogicalInstructions++;
    }
    else if(OPCode == "0110"){
        MEM_ALUOutput = "";
        for(int i=0;i<8;i++){
            if(regA[i] == '1') MEM_ALUOutput += '0';
            else MEM_ALUOutput += '1';
        }
        numLogicalInstructions++;
    }
    else if(OPCode == "0111"){
        MEM_ALUOutput = "";
        for(int i=0;i<8;i++){
            if(regA[i] == regB[i]) MEM_ALUOutput += '0';
            else MEM_ALUOutput += '1';
        }
        numLogicalInstructions++;
    }
    else if((OPCode == "1000") || (OPCode == "1001")){
        int result = BinaryCompToInt(regA) + BinaryCompToInt(regB);
        MEM_ALUOutput = IntToBinary(result,8);
        numDataInstructions++;
    }
    else if(OPCode == "1010"){
        stallEX = true;
        numControlInstructions++;
        MEM_ALUOutput = "";
        int result = BinaryCompToInt(regA)*2 + EX_PC;
        // cout<<"RESULT :"<<result<<endl;
        // cout<<"EX_PC :"<<EX_PC<<endl;
        // cout<<"INT VALUE OF TEMP :"<<BinaryToInt(temp)<<endl;
        MEM_ALUOutput = IntToBinary(result,16);
    }
    else if(OPCode == "1011"){
        // cout<<"CHECKING REGISTER : "<<BinaryToInt(regA)<<endl;
        stallEX = true;
        numControlInstructions++;
        MEM_ALUOutput = "";
        int result = EX_PC;
        if(BinaryToInt(regA) == 0) result += BinaryCompToInt(regB)*2;
        MEM_ALUOutput = IntToBinary(result,16);
    }

    MEM_IR = EX_IR;
    runMEM = true;
    runEX = false;
}

void Pipeline::MEM(){
    if(!runMEM || stallMEM) return;
    string OPCode = "";
    for(int i=0;i<4;i++) OPCode += MEM_IR[i];
    if((OPCode == "1010") || (OPCode == "1011")){
        if(IF_PC == BinaryToInt(MEM_ALUOutput)){
            runID = false;
            stallID = false;
            stallEX = false;
        }else{
            // cout<<MEM_ALUOutput<<" "<<BinaryToInt(MEM_ALUOutput)<<endl;
            IF_PC = BinaryToInt(MEM_ALUOutput);
            // cout<<"CHECK JMP ****** "<<IF_PC<<endl;
            flush();
        }
        runWB = false;
        return ;
    }
    else if(OPCode == "1001"){
        int index = BinaryToInt(MEM_ALUOutput);
        string tempReg = "";
        for(int i=4;i<8;i++) tempReg += MEM_IR[i];
        string data = R->registers[BinaryToInt(tempReg)];
        C->dCache[index] = data;

        runMEM = false;
        runWB = true;
        condWB = false;
        WB_ALUOutput = "";
        WB_IR = "";
        LMD = "";
        return;
    }
    else if(OPCode == "1000"){
        int index = BinaryToInt(MEM_ALUOutput);
        LMD = HexToBinary(C->dCache[index]);
        WB_IR = MEM_IR;
        WB_ALUOutput = MEM_ALUOutput;
        runWB = true;
    }
    else{
        runMEM = false;
        runWB = true;
        condWB = condMEM;
        WB_ALUOutput = MEM_ALUOutput;
        WB_IR = MEM_IR;
        LMD = "";
    }

    runMEM = false;
    return;
}

void Pipeline::WB(){
    if(!runWB || stallWB) return;
    string OPCode = "";
    string dest = "";
    for(int i=0;i<4;i++) OPCode += WB_IR[i];
    for(int i=4;i<8;i++) dest += WB_IR[i];
    int index = BinaryToInt(dest);
    if(OPCode == "1000"){
        R->registers[index] = BinaryToHex(LMD);
    }
    else{
        R->registers[index] = BinaryToHex(WB_ALUOutput);
    }
    R->status[index] = false;
    runWB = false;
}

void Pipeline::flush(){
    runIF = true;
    runID = false;
    runEX = false;
    runMEM = false;
    runWB = false;
    condWB = false;
    condMEM = false;

    stallIF = false;
    stallID = false;
    stallEX = false;
    stallMEM = false;
    stallWB = false;

    regA = "";
    regB = "";
    ID_IR = "";
    EX_IR = "";
    MEM_IR = "";
    WB_IR = "";
    MEM_ALUOutput = "";
    WB_ALUOutput = "";
    LMD = "";

    ID_PC = 0;
    EX_PC = 0;
}

int main(){
    fstream Iptr;
    fstream Dptr;
    fstream Rptr;
    Iptr.open("ICache.txt");
    Dptr.open("DCache.txt");
    Rptr.open("RF.txt");
	Cache tempcache;
	RegisterFile temprf;
	string s;
	vector<string> intr;
	while(Iptr>>s){
		intr.push_back(s);
	}
    Iptr.close();
	tempcache.getInstructions(intr);

	vector<string> data;
	while(Dptr>>s){
		data.push_back(s);
	}
    Dptr.close();
	tempcache.getData(data);

	vector<string> regis;
	while(Rptr>>s){
		regis.push_back(s);
	}
    Rptr.close();
	temprf.getRegisters(regis);


	Pipeline pipe;
	pipe.getCache(&tempcache);
	pipe.getRegisterFile(&temprf);

    pipe.run();
    pipe.printOutput();
	return 0;
}

