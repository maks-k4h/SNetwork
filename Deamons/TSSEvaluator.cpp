//
// Created by Maks Konevych on 03.05.2022.
//

#include "TSSEvaluator.h"
#include <chrono>
#include <iostream>
#include <ctime>
#include <cmath>

TSSEvaluator::TSSEvaluator(Server &s)
: server{s} {
    logfile.open(LOGP, std::ios_base::ate);
    if (!logfile.is_open()) {
        std::cerr << "Cannot open TSS Deamon logfile!\n";
    }
    mlog("Deamon created.");
}

TSSEvaluator::~TSSEvaluator() {
    stop();
    mlog("Deamon destroyed.");
    logfile.close();
}

bool TSSEvaluator::run() {
    if (running || thr) {
        elog("Try to run already running deamon.");
        return false;
    }
    mlog("Running the deamon.");
    running = true;
    counter = updateRateMS;
    thr = new std::thread(&TSSEvaluator::TSSf, this);
    mlog("The deamon is running.");
    return true;
}

void TSSEvaluator::stop() {
    if (running && thr) {
        mlog("Stopping the deamon.");
        running = false;
        thr->join();
        delete thr;
        thr = nullptr;
        mlog("The deamon is stopped.");
    }
}

void TSSEvaluator::TSSf() {
    while (running) {
        update();
        counter = updateRateMS;
        while (running) {
            if (counter <= 0)
                break;
            counter -= clockRateMS;
            std::this_thread::sleep_for(std::chrono::milliseconds(clockRateMS));
        }
    }
}

void TSSEvaluator::update() {
    auto unratedList = server.getUnrated();
    if (unratedList.empty()) {
        mlog("No messages to rate.");
    } else {
        mlog(std::to_string(unratedList.size()) + " messages to rate.");
        auto toUpdate = unratedList.front();

        mlog("Rating message with id " + toUpdate.toString());
        if (server.setRate(toUpdate, pow(1.0 * rand() / RAND_MAX, 4)))
            mlog("Message's rate successfully updated.");
        else
            mlog("Cannot rate the message.");
    }
}

void TSSEvaluator::elog(const std::string &m) {
    if (!logfile.is_open())
        return;
    char timestring[100];
    time_t now = time(nullptr);
    auto now_tm = localtime(&now);
    strftime(timestring, 60, "ERROR : %c", now_tm);
    free(now_tm);
    logfile << timestring << m << '\n';
}

void TSSEvaluator::mlog(const std::string &m) {
    if (!logfile.is_open())
        return;
    char timestring[100];
    time_t now = time(nullptr);
    auto now_tm = localtime(&now);
    strftime(timestring, 60, "STATUS : %c", now_tm);
    logfile << timestring << '\t' << m << '\n';
}


