// Cleaned
#ifndef SRC_HPP
#define SRC_HPP

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include "base.hpp"

using namespace std;

struct date {
    int year, month, day;
    date() = default;
    date(int y, int m, int d) : year(y), month(m), day(d) {}

    bool operator<(const date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }

    friend istream& operator>>(istream& is, date& d) {
        is >> d.year >> d.month >> d.day;
        return is;
    }

    long long to_days() const {
        return (long long)year * 360 + (long long)month * 30 + (long long)day;
    }
};

class mail : public object {
protected:
    string postmark;
    date send_date;
    date arrive_date;

public:
    mail() = default;
    mail(string _contain_, string _postmark_, date send_d, date arrive_d)
        : object(_contain_), postmark(_postmark_), send_date(send_d), arrive_date(arrive_d) {}

    virtual string send_status(int y, int m, int d) override {
        return "not send";
    }

    virtual string type() override {
        return "no type";
    }

    virtual void print() override {
        object::print();
        cout << "[mail] postmark: " << postmark << endl;
    }

    virtual void copy(object* o) override {
        mail* other = reinterpret_cast<mail*>(o);
        contain = other->contain;
        postmark = other->postmark;
        send_date = other->send_date;
        arrive_date = other->arrive_date;
    }
};

class air_mail : public mail {
protected:
    date take_off_date;
    date land_date;
    string airlines;

public:
    air_mail() = default;
    air_mail(string _contain_, string _postmark_, date send_d, date arrive_d, date take_off, date land, string _airline)
        : mail(_contain_, _postmark_, send_d, arrive_d), take_off_date(take_off), land_date(land), airlines(_airline) {}

    virtual string send_status(int y, int m, int d) override {
        date query_date(y, m, d);
        if (query_date < send_date) return "mail not send";
        if (query_date < take_off_date) return "wait in airport";
        if (query_date < land_date) return "in flight";
        if (query_date < arrive_date) return "already land";
        return "already arrive";
    }

    virtual string type() override {
        return "air";
    }

    virtual void print() override {
        mail::print();
        cout << "[air] airlines: " << airlines << endl;
    }

    virtual void copy(object* o) override {
        air_mail* other = reinterpret_cast<air_mail*>(o);
        mail::copy(o);
        take_off_date = other->take_off_date;
        land_date = other->land_date;
        airlines = other->airlines;
    }
};

class train_mail : public mail {
protected:
    string* station_name = nullptr;
    date* station_time = nullptr;
    int len = 0;

    void clear() {
        if (station_name) delete[] station_name;
        if (station_time) delete[] station_time;
        station_name = nullptr;
        station_time = nullptr;
        len = 0;
    }

public:
    train_mail() = default;
    train_mail(string _contain_, string _postmark_, date send_d, date arrive_d, string* sname, date* stime, int station_num)
        : mail(_contain_, _postmark_, send_d, arrive_d), len(station_num) {
        if (len > 0) {
            station_name = new string[len];
            station_time = new date[len];
            for (int i = 0; i < len; ++i) {
                station_name[i] = sname[i];
                station_time[i] = stime[i];
            }
        }
    }

    virtual ~train_mail() {
        clear();
    }

    virtual string send_status(int y, int m, int d) override {
        date query_date(y, m, d);
        if (query_date < send_date) return "mail not send";

        bool all_earlier = true;
        for (int i = 0; i < len; ++i) {
            if (!(station_time[i] < query_date)) {
                all_earlier = false;
                break;
            }
        }

        if (len > 0 && !(query_date < station_time[0])) {
            int last_idx = -1;
            for (int i = 0; i < len; ++i) {
                if (!(query_date < station_time[i])) {
                    last_idx = i;
                } else {
                    break;
                }
            }
            if (query_date < arrive_date) {
                return "at station " + station_name[last_idx];
            } else {
                return "already arrive";
            }
        }

        if (query_date < arrive_date) return "train will arrive soon";
        return "already arrive";
    }

    virtual string type() override {
        return "train";
    }

    virtual void print() override {
        mail::print();
        cout << "[train] station_num: " << len << endl;
    }

    virtual void copy(object* o) override {
        train_mail* other = reinterpret_cast<train_mail*>(o);
        mail::copy(o);
        clear();
        len = other->len;
        if (len > 0) {
            station_name = new string[len];
            station_time = new date[len];
            for (int i = 0; i < len; ++i) {
                station_name[i] = other->station_name[i];
                station_time[i] = other->station_time[i];
            }
        }
    }
};

class car_mail : public mail {
protected:
    int total_mile;
    string driver;

public:
    car_mail() = default;
    car_mail(string _contain_, string _postmark_, date send_d, date arrive_d, int mile, string _driver)
        : mail(_contain_, _postmark_, send_d, arrive_d), total_mile(mile), driver(_driver) {}

    virtual string send_status(int y, int m, int d) override {
        date query_date(y, m, d);
        if (query_date < send_date) return "mail not send";
        if (!(query_date < arrive_date)) return "already arrive";

        long long total_days = arrive_date.to_days() - send_date.to_days();
        long long used_days = query_date.to_days() - send_date.to_days();

        double current_mile = (double)used_days / (double)total_days * (double)total_mile;
        return to_string(current_mile);
    }

    virtual string type() override {
        return "car";
    }

    virtual void print() override {
        mail::print();
        cout << "[car] driver_name: " << driver << endl;
    }

    virtual void copy(object* o) override {
        car_mail* other = reinterpret_cast<car_mail*>(o);
        mail::copy(o);
        total_mile = other->total_mile;
        driver = other->driver;
    }
};

void obj_swap(object*& lhs, object*& rhs) {
    object* tmp = lhs;
    lhs = rhs;
    rhs = tmp;
}

#endif
