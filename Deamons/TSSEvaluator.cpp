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
    logfile.open(LOGP, LOGMODE);
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
        auto inTable = loadTextTable(unratedList);
        auto outTable = evaluateTextTable(inTable);
        uploadRates(outTable);
    }
}

void TSSEvaluator::elog(const std::string &m) const {
    if (!logfile.is_open())
        return;
    char timestring[100];
    time_t now = time(nullptr);
    auto now_tm = localtime(&now);
    strftime(timestring, 70, "ERROR : %c", now_tm);
    free(now_tm);
    logfile << timestring << m << '\n';
}

void TSSEvaluator::mlog(const std::string &m) const {
    if (!logfile.is_open())
        return;
    char timestring[100];
    time_t now = time(nullptr);
    auto now_tm = localtime(&now);
    strftime(timestring, 70, "STATUS : %c", now_tm);
    logfile << timestring << '\t' << m << '\n';
}

TSSEvaluator::id_text_table
TSSEvaluator::loadTextTable(const std::list<MessageID> &list) const {
    id_text_table res;
    auto curr = list.begin();
    while (curr != list.end()) {
        Message m;
        if(server.getMessageById(*curr, m)) {
            res.push_back({*curr, std::move(m.getText())});
        } else {
            elog("Cannot load text of " + curr->toString() + '.');
        }
        ++curr;
    }
    mlog("Loaded a table of " + std::to_string(res.size())
        + " rows from a list of " + std::to_string(list.size()) + " ids.");
    return std::move(res);
}

TSSEvaluator::id_rate_table
TSSEvaluator::evaluateTextTable(const TSSEvaluator::id_text_table &t) const {
    exportCSV(t, csvOutFile);

    system(PCC.c_str());

    auto res = importCSV(csvInFile);
    mlog("The table of " + std::to_string(t.size()) + " rows was rated.");
    return std::move(res);
}

void
ReplaceAll(std::string &str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

void
TSSEvaluator::exportCSV(TSSEvaluator::id_text_table table,
                        const std::string &path) const {
    std::ofstream out(path);
    if (!out.is_open())
        elog("Cannot open/create " + path + " for table export.");
    else {
        out << "id" << csvSeparator << "text" << '\n';
        for (auto row : table) {
            ReplaceAll(std::get<1>(row), "\"", "\"\"");
            out << '"' << std::get<0>(row).toString() << '"'
                << csvSeparator
                << '"' << std::get<1>(row) << '"'
                << '\n';
        }
    }
}

TSSEvaluator::id_rate_table
TSSEvaluator::importCSV(const std::string &path) const {
    id_rate_table res;
    std::ifstream in(csvInFile);
    if (!in.is_open()) {
        elog(std::string("Cannot open ") + csvInFile);
    } else {
        std::string buff;
        std::getline(in, buff); // columns
        while (std::getline(in, buff)) {
            int count {};
            MessageID id;
            count += id.fromString(buff);
            while (buff[count++] != ',' && count < buff.size());
            double rate = atof(buff.substr(count).c_str());

            res.push_back({std::move(id), rate});
        }
    }

    mlog("CSV import done : " + std::to_string(res.size()) + " rows");

    return std::move(res);
}

void TSSEvaluator::uploadRates(const TSSEvaluator::id_rate_table &table) {
    int count {};
    for (auto row : table) {
        if (server.setRate(std::get<0>(row), std::get<1>(row))) {
            ++count;
        } else {
            elog("Cannot upload rate of " + std::get<0>(row).toString() + '.');
        }
    }
    mlog("Uploaded " + std::to_string(count) +
        " rates from " + std::to_string(table.size()));
}



