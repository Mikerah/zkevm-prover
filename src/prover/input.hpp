#ifndef INPUT_HPP
#define INPUT_HPP

#include <nlohmann/json.hpp>
#include <gmpxx.h>
#include "config.hpp"
#include "public_inputs.hpp"
#include "goldilocks_base_field.hpp"
#include "compare_fe.hpp"
#include "database.hpp"

using json = nlohmann::json;

class Input
{
    Goldilocks &fr;
    void db2json (json &input, const std::map<string, vector<Goldilocks::Element>> &db, string name) const;
    void contractsBytecode2json (json &input, const std::map<string, vector<uint8_t>> &contractsBytecode, string name) const;
public:
    PublicInputs publicInputs;
    string globalExitRoot;
    string batchL2Data;
    uint64_t txsLen;
    mpz_class batchHashData;
    mpz_class globalHash; // Used by executor, not by gRPC server
    string from; // Used for unsigned transactions

    // Constructor
    Input(Goldilocks &fr) : fr(fr), txsLen(0) {};

    // Loads the input object data from a JSON object
    void load (json &input);

    // Saves the input object data into a JSON object
    void save (json &input) const;
    void save (json &input, const Database &database) const;

private:
    void loadGlobals      (json &input);
    void saveGlobals      (json &input) const;
#ifdef USE_LOCAL_STORAGE
public:
    map< Goldilocks::Element, mpz_class, CompareFe> sto; // Input JSON will include the initial values of the rellevant storage positions
    void loadStorage      (json &input);
    void saveStorage      (json &input) const;
#endif
public:
    //map< Goldilocks::Element, vector<Goldilocks::Element>, CompareFe > db; // This is in fact a map<fe,fe[16]>
    map< string, vector<Goldilocks::Element> > db; // This is in fact a map<fe,fe[16]>
    map< string, vector<uint8_t> > contractsBytecode;
    void loadDatabase     (json &input);
    void saveDatabase     (json &input) const;
    void saveDatabase     (json &input, const Database &database) const;
    void preprocessTxs    (void);
};

#endif