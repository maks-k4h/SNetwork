


#ifndef DEAMONS_TSSEVALUATOR_H_
#define DEAMONS_TSSEVALUATOR_H_

/*
 * TSSEvaluator — Text Spam Score Evaluation Deamon
 *
 * */

#include "../Server/Server.h"

#include <thread>
#include <fstream>
#include <vector>
#include <list>
#include <tuple>
#include <string>

// Python Script Path
const std::string PSP ("../Deamons/TSSScript.py");
// Python Call Command
const std::string PCC = {"python3 " + PSP};

// csv files
constexpr auto csvOutFile {"../Deamons/TSSData.csv"};
constexpr auto csvInFile {"../Deamons/TSSRates.csv"};
constexpr auto csvSeparator = ',';

// Deamon log filepath
constexpr auto LOGP {"../Deamons/TSSDeamonLog.txt"};
constexpr auto LOGMODE = std::ios_base::ate;

constexpr auto clockRateMS {500};
constexpr auto updateRateMS {5000};


class TSSEvaluator {
public:
    TSSEvaluator(Server &);
    ~TSSEvaluator();

    bool run();
    void stop();

private:
    std::thread *thr {};

    using id_text_table = std::vector<std::tuple<MessageID, std::string>>;
    using id_rate_table = std::vector<std::tuple<MessageID, double>>;

    void TSSf();
    void update();

    // Create the table from list of ids, perform requests to the server
    // |ID|         ID  | TEXT
    // ----  into   ----|-----
    // |..|         ... | ....
    id_text_table loadTextTable(const std::list<MessageID> &) const;

    // Create the table of evaluated messages, perform calls to python script
    // ID  | TEXT           ID  | TSS
    // ----|-----   into    ---------
    // ... | ....           ... | ...
    //
    id_rate_table evaluateTextTable(const id_text_table &) const;

    // csv files
    void exportCSV(id_text_table, const std::string &path) const;
    id_rate_table importCSV(const std::string &path) const;

    // upload rates to the server
    void uploadRates(const id_rate_table &);

    bool running {};
    int counter {};

    Server &server;
    mutable std::ofstream logfile;
    void elog(const std::string &) const;
    void mlog(const std::string &) const;

};


#endif // DEAMONS_TSSEVALUATOR_H_
