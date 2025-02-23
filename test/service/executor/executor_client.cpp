
#include <nlohmann/json.hpp>
#include "executor_client.hpp"

using namespace std;
using json = nlohmann::json;

ExecutorClient::ExecutorClient (Goldilocks &fr, const Config &config) :
    fr(fr),
    config(config)
{
    // Create channel
    std::shared_ptr<grpc_impl::Channel> channel = ::grpc::CreateChannel(config.executorClientHost + ":" + to_string(config.executorClientPort), grpc::InsecureChannelCredentials());

    // Create stub (i.e. client)
    stub = new executor::v1::ExecutorService::Stub(channel);
}

void ExecutorClient::runThread (void)
{
    pthread_create(&t, NULL, executorClientThread, this);
}

void ExecutorClient::waitForThread (void)
{
    pthread_join(t, NULL);
}

bool ExecutorClient::ProcessBatch (void)
{
    TimerStart(EXECUTOR_CLIENT_PROCESS_BATCH);

    if (config.inputFile.size() == 0)
    {
        cerr << "Error: ExecutorClient::ProcessBatch() found config.inputFile empty" << endl;
        exit(-1);
    }
    ::grpc::ClientContext context;
    ::executor::v1::ProcessBatchRequest request;
    Input input(fr);
    json inputJson;
    file2json(config.inputFile, inputJson);
    input.load(inputJson);
    //input.preprocessTxs();

    bool update_merkle_tree = true;

    request.set_batch_num(input.publicInputs.batchNum);
    request.set_coinbase(input.publicInputs.sequencerAddr);
    request.set_batch_l2_data(string2ba(input.batchL2Data));
    request.set_old_state_root(string2ba(input.publicInputs.oldStateRoot));
    request.set_old_local_exit_root(string2ba(input.publicInputs.oldLocalExitRoot));
    request.set_global_exit_root(string2ba(input.globalExitRoot));
    request.set_eth_timestamp(input.publicInputs.timestamp);
    request.set_update_merkle_tree(update_merkle_tree);

    // Parse keys map
    map< string, vector<Goldilocks::Element>>::const_iterator it;
    for (it=input.db.begin(); it!=input.db.end(); it++)
    {
        string key = NormalizeToNFormat(it->first, 64);
        string value;
        vector<Goldilocks::Element> dbValue = it->second;
        for (uint64_t i=0; i<dbValue.size(); i++)
        {
            value += NormalizeToNFormat(fr.toString(dbValue[i], 16), 16);
        }
        (*request.mutable_db())[key] = value;
    }

    // Parse contracts data
    map< string, vector<uint8_t>>::const_iterator itp;
    for (itp=input.contractsBytecode.begin(); itp!=input.contractsBytecode.end(); itp++)
    {
        string key = NormalizeToNFormat(itp->first, 64);
        string value;
        vector<uint8_t> contractValue = itp->second;
        for (uint64_t i=0; i<contractValue.size(); i++)
        {
            value += byte2string(contractValue[i]);
        }
        (*request.mutable_contracts_bytecode())[key] = value;
    }    

    ::executor::v1::ProcessBatchResponse response;
    std::unique_ptr<grpc::ClientReaderWriter<executor::v1::ProcessBatchRequest, executor::v1::ProcessBatchResponse>> readerWriter;
    stub->ProcessBatch(&context, request, &response);
    
    cout << "ExecutorClient::ProcessBatch() got:\n" << response.DebugString() << endl;

    TimerStopAndLog(EXECUTOR_CLIENT_PROCESS_BATCH);

    return true;
}

void* executorClientThread(void* arg)
{
    cout << "executorClientThread() started" << endl;
    string uuid;
    ExecutorClient *pClient = (ExecutorClient *)arg;

    // Allow service to initialize
    sleep(1);

    // Execute should block and succeed
    cout << "executorClientThread() calling Execute()" << endl;
    pClient->ProcessBatch();
    return NULL;
}