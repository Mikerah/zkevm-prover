#include "stark_info.hpp"
#include "utils.hpp"
#include "timer.hpp"

StarkInfo::StarkInfo(const Config &config) : config(config)
{
    // Avoid initialization if we are not going to generate any proof
    if (!config.generateProof()) return;

    // Load contents from json file
    TimerStart(STARK_INFO_LOAD);
    json starkInfoJson;
    file2json(config.starkInfoFile, starkInfoJson);
    load(starkInfoJson);
    TimerStopAndLog(STARK_INFO_LOAD);
}

void StarkInfo::load (json j)
{
    starkStruct.nBits = j["starkStruct"]["nBits"];
    starkStruct.nBitsExt = j["starkStruct"]["nBitsExt"];
    starkStruct.nQueries = j["starkStruct"]["nQueries"];
    starkStruct.verificationHashType = j["starkStruct"]["verificationHashType"];
    for (uint64_t i=0; i<j["starkStruct"]["steps"].size(); i++)
    {
        StepStruct step;
        step.nBits = j["starkStruct"]["steps"][i]["nBits"];
        starkStruct.steps.push_back(step);
    }

    mapTotalN = j["mapTotalN"];
    nConstants = j["nConstants"];
    nCm1 = j["nCm1"];
    nCm2 = j["nCm2"];
    nCm3 = j["nCm3"];
    nCm4 = j["nCm4"];
    nQ = j["nQ"];
    friExpId = j["friExpId"];
    nExps = j["nExps"];

    mapDeg.section[cm1_n] = j["mapDeg"]["cm1_n"];
    mapDeg.section[cm2_n] = j["mapDeg"]["cm2_n"];
    mapDeg.section[cm3_n] = j["mapDeg"]["cm3_n"];
    mapDeg.section[exps_withq_n] = j["mapDeg"]["exps_withq_n"];
    mapDeg.section[exps_withoutq_n] = j["mapDeg"]["exps_withoutq_n"];
    mapDeg.section[cm1_2ns] = j["mapDeg"]["cm1_2ns"];
    mapDeg.section[cm2_2ns] = j["mapDeg"]["cm2_2ns"];
    mapDeg.section[cm3_2ns] = j["mapDeg"]["cm3_2ns"];
    mapDeg.section[q_2ns] = j["mapDeg"]["q_2ns"];
    mapDeg.section[exps_withq_2ns] = j["mapDeg"]["exps_withq_2ns"];
    mapDeg.section[exps_withoutq_2ns] = j["mapDeg"]["exps_withoutq_2ns"];

    mapOffsets.section[cm1_n] = j["mapOffsets"]["cm1_n"];
    mapOffsets.section[cm2_n] = j["mapOffsets"]["cm2_n"];
    mapOffsets.section[cm3_n] = j["mapOffsets"]["cm3_n"];
    mapOffsets.section[exps_withq_n] = j["mapOffsets"]["exps_withq_n"];
    mapOffsets.section[exps_withoutq_n ]= j["mapOffsets"]["exps_withoutq_n"];
    mapOffsets.section[cm1_2ns] = j["mapOffsets"]["cm1_2ns"];
    mapOffsets.section[cm2_2ns] = j["mapOffsets"]["cm2_2ns"];
    mapOffsets.section[cm3_2ns] = j["mapOffsets"]["cm3_2ns"];
    mapOffsets.section[q_2ns] = j["mapOffsets"]["q_2ns"];
    mapOffsets.section[exps_withq_2ns] = j["mapOffsets"]["exps_withq_2ns"];
    mapOffsets.section[exps_withoutq_2ns] = j["mapOffsets"]["exps_withoutq_2ns"];

    for (uint64_t i=0; i<j["mapSections"]["cm1_n"].size(); i++) mapSections.section[cm1_n].push_back(j["mapSections"]["cm1_n"][i]);
    for (uint64_t i=0; i<j["mapSections"]["cm2_n"].size(); i++) mapSections.section[cm2_n].push_back(j["mapSections"]["cm2_n"][i]);
    for (uint64_t i=0; i<j["mapSections"]["cm3_n"].size(); i++) mapSections.section[cm3_n].push_back(j["mapSections"]["cm3_n"][i]);
    for (uint64_t i=0; i<j["mapSections"]["exps_withq_n"].size(); i++) mapSections.section[exps_withq_n].push_back(j["mapSections"]["exps_withq_n"][i]);
    for (uint64_t i=0; i<j["mapSections"]["exps_withoutq_n"].size(); i++) mapSections.section[exps_withoutq_n].push_back(j["mapSections"]["exps_withoutq_n"][i]);
    for (uint64_t i=0; i<j["mapSections"]["cm1_2ns"].size(); i++) mapSections.section[cm1_2ns].push_back(j["mapSections"]["cm1_2ns"][i]);
    for (uint64_t i=0; i<j["mapSections"]["cm2_2ns"].size(); i++) mapSections.section[cm2_2ns].push_back(j["mapSections"]["cm2_2ns"][i]);
    for (uint64_t i=0; i<j["mapSections"]["cm3_2ns"].size(); i++) mapSections.section[cm3_2ns].push_back(j["mapSections"]["cm3_2ns"][i]);
    for (uint64_t i=0; i<j["mapSections"]["q_2ns"].size(); i++) mapSections.section[q_2ns].push_back(j["mapSections"]["q_2ns"][i]);
    for (uint64_t i=0; i<j["mapSections"]["exps_withq_2ns"].size(); i++) mapSections.section[exps_withq_2ns].push_back(j["mapSections"]["exps_withq_2ns"][i]);
    for (uint64_t i=0; i<j["mapSections"]["exps_withoutq_2ns"].size(); i++) mapSections.section[exps_withoutq_2ns].push_back(j["mapSections"]["exps_withoutq_2ns"][i]);

    mapSectionsN.section[cm1_n] = j["mapSectionsN"]["cm1_n"];
    mapSectionsN.section[cm2_n] = j["mapSectionsN"]["cm2_n"];
    mapSectionsN.section[cm3_n] = j["mapSectionsN"]["cm3_n"];
    mapSectionsN.section[exps_withq_n] = j["mapSectionsN"]["exps_withq_n"];
    mapSectionsN.section[exps_withoutq_n] = j["mapSectionsN"]["exps_withoutq_n"];
    mapSectionsN.section[cm1_2ns] = j["mapSectionsN"]["cm1_2ns"];
    mapSectionsN.section[cm2_2ns] = j["mapSectionsN"]["cm2_2ns"];
    mapSectionsN.section[cm3_2ns] = j["mapSectionsN"]["cm3_2ns"];
    mapSectionsN.section[q_2ns] = j["mapSectionsN"]["q_2ns"];
    mapSectionsN.section[exps_withq_2ns] = j["mapSectionsN"]["exps_withq_2ns"];
    mapSectionsN.section[exps_withoutq_2ns] = j["mapSectionsN"]["exps_withoutq_2ns"];

    mapSectionsN1.section[cm1_n] = j["mapSectionsN1"]["cm1_n"];
    mapSectionsN1.section[cm2_n] = j["mapSectionsN1"]["cm2_n"];
    mapSectionsN1.section[cm3_n] = j["mapSectionsN1"]["cm3_n"];
    mapSectionsN1.section[exps_withq_n] = j["mapSectionsN1"]["exps_withq_n"];
    mapSectionsN1.section[exps_withoutq_n] = j["mapSectionsN1"]["exps_withoutq_n"];
    mapSectionsN1.section[cm1_2ns] = j["mapSectionsN1"]["cm1_2ns"];
    mapSectionsN1.section[cm2_2ns] = j["mapSectionsN1"]["cm2_2ns"];
    mapSectionsN1.section[cm3_2ns] = j["mapSectionsN1"]["cm3_2ns"];
    mapSectionsN1.section[q_2ns] = j["mapSectionsN1"]["q_2ns"];
    mapSectionsN1.section[exps_withq_2ns] = j["mapSectionsN1"]["exps_withq_2ns"];
    mapSectionsN1.section[exps_withoutq_2ns] = j["mapSectionsN1"]["exps_withoutq_2ns"];

    mapSectionsN3.section[cm1_n] = j["mapSectionsN3"]["cm1_n"];
    mapSectionsN3.section[cm2_n] = j["mapSectionsN3"]["cm2_n"];
    mapSectionsN3.section[cm3_n] = j["mapSectionsN3"]["cm3_n"];
    mapSectionsN3.section[exps_withq_n] = j["mapSectionsN3"]["exps_withq_n"];
    mapSectionsN3.section[exps_withoutq_n] = j["mapSectionsN3"]["exps_withoutq_n"];
    mapSectionsN3.section[cm1_2ns] = j["mapSectionsN3"]["cm1_2ns"];
    mapSectionsN3.section[cm2_2ns] = j["mapSectionsN3"]["cm2_2ns"];
    mapSectionsN3.section[cm3_2ns] = j["mapSectionsN3"]["cm3_2ns"];
    mapSectionsN3.section[q_2ns] = j["mapSectionsN3"]["q_2ns"];
    mapSectionsN3.section[exps_withq_2ns] = j["mapSectionsN3"]["exps_withq_2ns"];
    mapSectionsN3.section[exps_withoutq_2ns] = j["mapSectionsN3"]["exps_withoutq_2ns"];

    for (uint64_t i=0; i<j["varPolMap"].size(); i++)
    {
        VarPolMap map;
        map.section = string2section(j["varPolMap"][i]["section"]);
        map.sectionPos = j["varPolMap"][i]["sectionPos"];
        map.dim = j["varPolMap"][i]["dim"];
        varPolMap.push_back(map);
    }

    for (uint64_t i=0; i<j["qs"].size(); i++) qs.push_back(j["qs"][i]);

    for (uint64_t i=0; i<j["cm_n"].size(); i++) cm_n.push_back(j["cm_n"][i]);

    for (uint64_t i=0; i<j["cm_2ns"].size(); i++) cm_2ns.push_back(j["cm_2ns"][i]);

    for (uint64_t i=0; i<j["peCtx"].size(); i++)
    {
        PeCtx pe;
        pe.tExpId = j["peCtx"][i]["tExpId"];
        pe.fExpId = j["peCtx"][i]["fExpId"];
        pe.zId = j["peCtx"][i]["zId"];
        pe.c1Id = j["peCtx"][i]["c1Id"];
        pe.numId = j["peCtx"][i]["numId"];
        pe.denId = j["peCtx"][i]["denId"];
        pe.c2Id = j["peCtx"][i]["c2Id"];
        peCtx.push_back(pe);
    }

    for (uint64_t i=0; i<j["puCtx"].size(); i++)
    {
        PuCtx pu;
        pu.tExpId = j["puCtx"][i]["tExpId"];
        pu.fExpId = j["puCtx"][i]["fExpId"];
        pu.h1Id = j["puCtx"][i]["h1Id"];
        pu.h2Id = j["puCtx"][i]["h2Id"];
        pu.zId = j["puCtx"][i]["zId"];
        pu.c1Id = j["puCtx"][i]["c1Id"];
        pu.numId = j["puCtx"][i]["numId"];
        pu.denId = j["puCtx"][i]["denId"];
        pu.c2Id = j["puCtx"][i]["c2Id"];
        puCtx.push_back(pu);
    }

    for (uint64_t i=0; i<j["ciCtx"].size(); i++)
    {
        CiCtx ci;
        ci.zId = j["ciCtx"][i]["zId"];
        ci.numId = j["ciCtx"][i]["numId"];
        ci.denId = j["ciCtx"][i]["denId"];
        ci.c1Id = j["ciCtx"][i]["c1Id"];
        ci.c2Id = j["ciCtx"][i]["c2Id"];
        ciCtx.push_back(ci);
    }

    for (uint64_t i=0; i<j["evMap"].size(); i++)
    {
        EvMap map;
        map.setType(j["evMap"][i]["type"]);
        map.id = j["evMap"][i]["id"];
        map.prime = j["evMap"][i]["prime"];
        evMap.push_back(map);
    }

    step2prev.tmpUsed = j["step2prev"]["tmpUsed"];
    for (uint64_t i=0; i<j["step2prev"]["first"].size(); i++)
    {
        StepOperation op;
        op.setOperation(j["step2prev"]["first"][i]["op"]); // Mandatory field
        op.dest.setType(j["step2prev"]["first"][i]["dest"]["type"]); // Mandatory field
        op.dest.id = j["step2prev"]["first"][i]["dest"]["id"]; // Mandatory field
        if (j["step2prev"]["first"][i]["dest"].contains("prime")) op.dest.prime = j["step2prev"]["first"][i]["dest"]["prime"]; else op.dest.prime = false;
        if (j["step2prev"]["first"][i]["dest"].contains("p")) op.dest.p = j["step2prev"]["first"][i]["dest"]["p"]; else op.dest.p = 0;
        for (uint64_t k=0; k<j["step2prev"]["first"][i]["src"].size(); k++)
        {
            StepType ty;
            ty.setType(j["step2prev"]["first"][i]["src"][k]["type"]); // Mandatory field
            if (j["step2prev"]["first"][i]["src"][k].contains("id")) ty.id = j["step2prev"]["first"][i]["src"][k]["id"]; else ty.id=0; // Mandatory field
            if (j["step2prev"]["first"][i]["src"][k].contains("prime")) ty.prime = j["step2prev"]["first"][i]["src"][k]["prime"]; else ty.prime = false;
            if (j["step2prev"]["first"][i]["src"][k].contains("p")) ty.p = j["step2prev"]["first"][i]["src"][k]["p"]; else ty.p = 0;
            if (j["step2prev"]["first"][i]["src"][k].contains("value")) ty.value = j["step2prev"]["first"][i]["src"][k]["value"];
            op.src.push_back(ty);
        }
        step2prev.first.push_back(op);
    }

    step3prev.tmpUsed = j["step3prev"]["tmpUsed"];
    for (uint64_t i=0; i<j["step3prev"]["first"].size(); i++)
    {
        StepOperation op;
        op.setOperation(j["step3prev"]["first"][i]["op"]); // Mandatory field
        op.dest.setType(j["step3prev"]["first"][i]["dest"]["type"]); // Mandatory field
        op.dest.id = j["step3prev"]["first"][i]["dest"]["id"]; // Mandatory field
        if (j["step3prev"]["first"][i]["dest"].contains("prime")) op.dest.prime = j["step3prev"]["first"][i]["dest"]["prime"]; else op.dest.prime = false;
        if (j["step3prev"]["first"][i]["dest"].contains("p")) op.dest.p = j["step3prev"]["first"][i]["dest"]["p"]; else op.dest.p = 0;
        for (uint64_t k=0; k<j["step3prev"]["first"][i]["src"].size(); k++)
        {
            StepType ty;
            ty.setType(j["step3prev"]["first"][i]["src"][k]["type"]); // Mandatory field
            if (j["step3prev"]["first"][i]["src"][k].contains("id")) ty.id = j["step3prev"]["first"][i]["src"][k]["id"]; else ty.id=0; // Mandatory field
            if (j["step3prev"]["first"][i]["src"][k].contains("prime")) ty.prime = j["step3prev"]["first"][i]["src"][k]["prime"]; else ty.prime = false;
            if (j["step3prev"]["first"][i]["src"][k].contains("p")) ty.p = j["step3prev"]["first"][i]["src"][k]["p"]; else ty.p = 0;
            if (j["step3prev"]["first"][i]["src"][k].contains("value")) ty.value = j["step3prev"]["first"][i]["src"][k]["value"];
            op.src.push_back(ty);
        }
        step3prev.first.push_back(op);
    }

    step4.tmpUsed = j["step4"]["tmpUsed"];
    for (uint64_t i=0; i<j["step4"]["first"].size(); i++)
    {
        StepOperation op;
        op.setOperation(j["step4"]["first"][i]["op"]); // Mandatory field
        op.dest.setType(j["step4"]["first"][i]["dest"]["type"]); // Mandatory field
        op.dest.id = j["step4"]["first"][i]["dest"]["id"]; // Mandatory field
        if (j["step4"]["first"][i]["dest"].contains("prime")) op.dest.prime = j["step4"]["first"][i]["dest"]["prime"]; else op.dest.prime = false;
        if (j["step4"]["first"][i]["dest"].contains("p")) op.dest.p = j["step4"]["first"][i]["dest"]["p"]; else op.dest.p = 0;
        for (uint64_t k=0; k<j["step4"]["first"][i]["src"].size(); k++)
        {
            StepType ty;
            ty.setType(j["step4"]["first"][i]["src"][k]["type"]); // Mandatory field
            if (j["step4"]["first"][i]["src"][k].contains("id")) ty.id = j["step4"]["first"][i]["src"][k]["id"]; else ty.id=0; // Mandatory field
            if (j["step4"]["first"][i]["src"][k].contains("prime")) ty.prime = j["step4"]["first"][i]["src"][k]["prime"]; else ty.prime = false;
            if (j["step4"]["first"][i]["src"][k].contains("p")) ty.p = j["step4"]["first"][i]["src"][k]["p"]; else ty.p = 0;
            if (j["step4"]["first"][i]["src"][k].contains("value")) ty.value = j["step4"]["first"][i]["src"][k]["value"];
            op.src.push_back(ty);
        }
        step4.first.push_back(op);
    }

    step42ns.tmpUsed = j["step42ns"]["tmpUsed"];
    for (uint64_t i=0; i<j["step42ns"]["first"].size(); i++)
    {
        StepOperation op;
        op.setOperation(j["step42ns"]["first"][i]["op"]); // Mandatory field
        op.dest.setType(j["step42ns"]["first"][i]["dest"]["type"]); // Mandatory field
        op.dest.id = j["step42ns"]["first"][i]["dest"]["id"]; // Mandatory field
        if (j["step42ns"]["first"][i]["dest"].contains("prime")) op.dest.prime = j["step42ns"]["first"][i]["dest"]["prime"]; else op.dest.prime = false;
        if (j["step42ns"]["first"][i]["dest"].contains("p")) op.dest.p = j["step42ns"]["first"][i]["dest"]["p"]; else op.dest.p = 0;
        for (uint64_t k=0; k<j["step42ns"]["first"][i]["src"].size(); k++)
        {
            StepType ty;
            ty.setType(j["step42ns"]["first"][i]["src"][k]["type"]); // Mandatory field
            if (j["step42ns"]["first"][i]["src"][k].contains("id")) ty.id = j["step42ns"]["first"][i]["src"][k]["id"]; else ty.id=0; // Mandatory field
            if (j["step42ns"]["first"][i]["src"][k].contains("prime")) ty.prime = j["step42ns"]["first"][i]["src"][k]["prime"]; else ty.prime = false;
            if (j["step42ns"]["first"][i]["src"][k].contains("p")) ty.p = j["step42ns"]["first"][i]["src"][k]["p"]; else ty.p = 0;
            if (j["step42ns"]["first"][i]["src"][k].contains("value")) ty.value = j["step42ns"]["first"][i]["src"][k]["value"];
            op.src.push_back(ty);
        }
        step42ns.first.push_back(op);
    }

    step52ns.tmpUsed = j["step52ns"]["tmpUsed"];
    for (uint64_t i=0; i<j["step52ns"]["first"].size(); i++)
    {
        StepOperation op;
        
        if (j["step52ns"]["first"][i]["op"] == "add") op.op = StepOperation::add;
        else if (j["step52ns"]["first"][i]["op"] == "sub") op.op = StepOperation::sub;
        else if (j["step52ns"]["first"][i]["op"] == "mul") op.op = StepOperation::mul;
        else if (j["step52ns"]["first"][i]["op"] == "copy") op.op = StepOperation::copy;
        else
        {
            cerr << "Error: StarkInfo::load() found invalid value of step operation: " << j["step52ns"]["first"][i]["op"] << endl;
            exit(-1);
        }

        op.dest.setType(j["step52ns"]["first"][i]["dest"]["type"]); // Mandatory field
        op.dest.id = j["step52ns"]["first"][i]["dest"]["id"]; // Mandatory field
        if (j["step52ns"]["first"][i]["dest"].contains("prime")) op.dest.prime = j["step52ns"]["first"][i]["dest"]["prime"]; else op.dest.prime = false;
        if (j["step52ns"]["first"][i]["dest"].contains("p")) op.dest.p = j["step52ns"]["first"][i]["dest"]["p"]; else op.dest.p = 0;
        for (uint64_t k=0; k<j["step52ns"]["first"][i]["src"].size(); k++)
        {
            StepType ty;
            ty.setType(j["step52ns"]["first"][i]["src"][k]["type"]); // Mandatory field
            if (j["step52ns"]["first"][i]["src"][k].contains("id")) ty.id = j["step52ns"]["first"][i]["src"][k]["id"]; else ty.id=0; // Mandatory field
            if (j["step52ns"]["first"][i]["src"][k].contains("prime")) ty.prime = j["step52ns"]["first"][i]["src"][k]["prime"]; else ty.prime = false;
            if (j["step52ns"]["first"][i]["src"][k].contains("p")) ty.p = j["step52ns"]["first"][i]["src"][k]["p"]; else ty.p = 0;
            if (j["step52ns"]["first"][i]["src"][k].contains("value")) ty.value = j["step52ns"]["first"][i]["src"][k]["value"];
            op.src.push_back(ty);
        }
        step52ns.first.push_back(op);
    }

    for (uint64_t i=0; i<j["exps_n"].size(); i++)
    {
        uint64_t value = 0;
        if (!j["exps_n"][i].is_null())
        {
            value = j["exps_n"][i];
        }
        exps_n.push_back(value);
    }

    for (uint64_t i=0; i<j["exps_2ns"].size(); i++)
    {
        uint64_t value = 0;
        if (!j["exps_2ns"][i].is_null())
        {
            value = j["exps_2ns"][i];
        }
        exps_2ns.push_back(value);
    }

}

void StarkInfo::getPol(void * pAddress, uint64_t idPol, PolInfo &polInfo)
{
    polInfo.map = varPolMap[idPol];
    polInfo.N = mapDeg.section[polInfo.map.section];
    polInfo.offset = mapOffsets.section[polInfo.map.section];
    polInfo.offset += polInfo.map.sectionPos;
    polInfo.size = mapSectionsN.section[polInfo.map.section];
    polInfo.pAddress = ((Goldilocks::Element *)pAddress) + polInfo.offset;
}

uint64_t StarkInfo::getPolSize(uint64_t polId)
{
        VarPolMap p = varPolMap[polId];
        uint64_t N = mapDeg.section[p.section];
        return N * p.dim * sizeof(Goldilocks::Element);
}

Polinomial StarkInfo::getPolinomial(Goldilocks::Element *pAddress, uint64_t idPol)
{
    VarPolMap polInfo = varPolMap[idPol];
    uint64_t dim = polInfo.dim;
    uint64_t N = mapDeg.section[polInfo.section];
    uint64_t offset = mapOffsets.section[polInfo.section];
    offset += polInfo.sectionPos;
    uint64_t next = mapSectionsN.section[polInfo.section];
    return Polinomial(&pAddress[offset], N, dim, next, std::to_string(idPol));
}

eSection string2section (const string s)
{
    if (s == "cm1_n") return cm1_n;
    if (s == "cm2_n") return cm2_n;
    if (s == "cm3_n") return cm3_n;
    if (s == "exps_withq_n") return exps_withq_n;
    if (s == "exps_withoutq_n") return exps_withoutq_n;
    if (s == "cm1_2ns") return cm1_2ns;
    if (s == "cm2_2ns") return cm2_2ns;
    if (s == "cm3_2ns") return cm3_2ns;
    if (s == "exps_withq_2ns") return exps_withq_2ns;
    if (s == "exps_withoutq_2ns") return exps_withoutq_2ns;
    if (s == "q_2ns") return q_2ns;
    cerr << "Error: string2section() found invalid string=" << s << endl;
    exit(-1);
}