#include "keccak.hpp"
#include "timer.hpp"

/*
    Input is a buffer of any length, including 0
    Output is 256 bits long buffer containing the 32B keccak hash of the input
*/
void Keccak (const uint8_t * pInput, uint64_t inputSize, uint8_t * pOutput, string scriptFile, string polsFile, string connectionsFile)
{
    Keccak2Input input;
    input.init(pInput, inputSize);
    KeccakState S;

    uint8_t r[1088];
    while (input.getNextBits(r))
    {
        for (uint64_t i=0; i<1088; i++)
        {
            S.gate[SinRef0 + i*9].pin[pin_a].bit ^= r[i];
        }
        KeccakF(S);
        S.printCounters();

        // Generate the script file only after the first keccak-f round
        if (scriptFile.size() > 0)
        {
            json j;
            S.saveScriptToJson(j);
            cout << "Generating Keccak script file: " << scriptFile << endl;
            json2file(j, scriptFile);
            cout << "Generated Keccak script file: " << scriptFile << endl;
            scriptFile = "";
        }

        // Generate the polynomials file only after the first keccak-f round
        if (polsFile.size() > 0)
        {
            json j;
            S.savePolsToJson(j);
            cout << "Generating Keccak polynomials file: " << polsFile << endl;
            json2file(j, polsFile);
            cout << "Generated Keccak polynomials file: " << polsFile << endl;
            polsFile = "";
        }

        // Generate the connections file only after the first keccak-f round
        if (connectionsFile.size() > 0)
        {
            json j;
            S.saveConnectionsToJson(j);
            cout << "Generating Keccak connections file: " << connectionsFile << endl;
            json2file(j, connectionsFile);
            cout << "Generated Keccak connections file: " << connectionsFile << endl;
            connectionsFile = "";
        }
        
        S.copySoutToSinAndResetRefs();
    }
    S.getOutput(pOutput);
}

void KeccakGenerateScript (const Config & config)
{
    TimerStart(KECCAK_SM_GENERATE_SCRIPT);
    uint8_t hash[32];
    Keccak(NULL, 0, hash, config.keccakScriptFile, config.keccakPolsFile, config.keccakConnectionsFile);
    TimerStopAndLog(KECCAK_SM_GENERATE_SCRIPT);
}

/* Unit test */
void KeccakSMTest (void)
{
    cout << "KeccakSMTest() starting" << endl;

    /* Use a well-known input */
    uint8_t input[188] = {
        0x09, 0x0B, 0xCA, 0xF7, 0x34, 0xC4, 0xF0, 0x6C, 0x93, 0x95,
        0x4A, 0x82, 0x7B, 0x45, 0xA6, 0xE8, 0xC6, 0x7B, 0x8E, 0x0F, 
        0xD1, 0xE0, 0xA3, 0x5A, 0x1C, 0x59, 0x82, 0xD6, 0x96, 0x18, 
        0x28, 0xF9, 0x09, 0x0B, 0xCA, 0xF7, 0x34, 0xC4, 0xF0, 0x6C, 
        0x93, 0x95, 0x4A, 0x82, 0x7B, 0x45, 0xA6, 0xE8, 0xC6, 0x7B, 

        0x8E, 0x0F, 0xD1, 0xE0, 0xA3, 0x5A, 0x1C, 0x59, 0x82, 0xD6, 
        0x96, 0x18, 0x28, 0xF9, 0x09, 0x0B, 0xCA, 0xF7, 0x34, 0xC4, 
        0xF0, 0x6C, 0x93, 0x95, 0x4A, 0x82, 0x7B, 0x45, 0xA6, 0xE8, 
        0xC6, 0x7B, 0x8E, 0x0F, 0xD1, 0xE0, 0xA3, 0x5A, 0x1C, 0x59, 
        0x82, 0xD6, 0x96, 0x18, 0x28, 0xF9, 0x17, 0xC0, 0x4C, 0x37, 

        0x60, 0x51, 0x0B, 0x48, 0xC6, 0x01, 0x27, 0x42, 0xC5, 0x40, 
        0xA8, 0x1A, 0xBA, 0x4B, 0xCA, 0x2F, 0x78, 0xB9, 0xD1, 0x4B, 
        0xFD, 0x2F, 0x12, 0x3E, 0x2E, 0x53, 0xEA, 0x3E, 0x61, 0x7B, 
        0x3A, 0x35, 0x28, 0xF9, 0xCD, 0xD6,   0x63, 0x0F, 0xD3, 0x30, 
        0x1B, 0x9C, 0x89, 0x11, 0xF7, 0xBF, 0x06, 0x3D, 0x29, 0x90,

        0x27, 0xCC, 0x1E, 0xE6, 0x56, 0x7E, 0x0F, 0xE5, 0xD6, 0x64, 
        0x87, 0x11, 0x82, 0xE4, 0xC6, 0xEA, 0xDA, 0xE6, 0x1A, 0x17, 
        0x06, 0xD8, 0x6D, 0x27, 0x32, 0x1A, 0xC3, 0x24, 0x6F, 0x98, 
        0x00, 0x00, 0x03, 0xE9, 0x00, 0x00, 0x00, 0x01};

    uint64_t inputSize = 188;

    /* Call Keccak to get the hash of the input */
    TimerStart(KECCAK_SM);
    uint8_t hash[32];
    Keccak(input, inputSize, hash);
    TimerStopAndLog(KECCAK_SM);
    printBa(hash, 32, "hash");    // Expected result: hash:0x1AFD6EAF13538380D99A245C2ACC4A25481B54556AE080CF07D1FACC0638CD8E

    /* Call the current Keccak to compare */
    TimerStart(CURRENT_KECCAK);
    string aux = keccak256(input, inputSize);
    TimerStopAndLog(CURRENT_KECCAK);
    cout << "Current Keccak: " << aux << endl;
}