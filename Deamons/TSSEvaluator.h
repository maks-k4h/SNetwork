


#ifndef DEAMONS_TSSEVALUATOR_H_
#define DEAMONS_TSSEVALUATOR_H_

/*
 * TSSEvaluator — Text Spam Score Evaluation Deamon
 *
 * */

#include "../Server/Server.h"

#include <thread>
#include <fstream>

// Python Script Path
constexpr auto PSP {"script.py"};

// Deamon log filepath
constexpr auto LOGP {"TSSDeamonLog.txt"};

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

    void TSSf();
    void update();

    bool running {};
    int counter {};

    Server &server;

    std::ofstream logfile;
    void elog(const std::string &);
    void mlog(const std::string &);

};


#endif // DEAMONS_TSSEVALUATOR_H_
