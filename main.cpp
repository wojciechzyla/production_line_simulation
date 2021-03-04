//
// Created by Wojciech Żyła on 15/12/2020.
//

#include <iostream>
#include "package.hpp"
#include "storage_types.hpp"
#include "types.hpp"
#include "vector"
#include "nodes.hpp"
#include <optional>
#include "factory.hpp"
#include <iostream>
#include <fstream>
#include "reports.hpp"
#include "simulation.hpp"

int main()
{
    /*
    std::ifstream file("/Users/wojciechzyla/Desktop/AGH/3_semestr/ZPO/netsim-tres_espadroles/plik.txt", std::ios::in);
    if(file.is_open()){
        Factory factory = load_factory_structure(file);
        file.close();

        IntervalReportNotifier interval(5);
        std::set<Time> spec = {3, 2, 6};
        SpecificTurnsReportNotifier specific(spec);
        simulate(factory, 6, [&interval](Factory& f, TimeOffset t_off) {if (interval.should_generate_report(t_off)) generate_simulation_turn_report(f, std::cout, t_off);});
        //simulate(factory, 6, [&specific](Factory& f, TimeOffset t_off) {if (specific.should_generate_report(t_off)) generate_simulation_turn_report(f, std::cout, t_off);});
    }
    else{
        std::cout<<"Nie otwarlem";
    }*/

    /*
    std::ifstream file4("/Users/wojciechzyla/Desktop/AGH/3_semestr/ZPO/netsim-tres_espadroles/plik.txt", std::ios::in);
    Factory fact_save3 = load_factory_structure(file4);
    std::ofstream reportsimulation("/Users/wojciechzyla/Desktop/AGH/3_semestr/ZPO/netsim-tres_espadroles/report_simulation.txt", std::ios::out);
    std::ofstream reportsimulation2("/Users/wojciechzyla/Desktop/AGH/3_semestr/ZPO/netsim-tres_espadroles/report_simulation2.txt", std::ios::out);

    Time rounds = 3;
     for (Time i = 1; i <= rounds; i++) {
         fact_save3.do_deliveries(i);
         fact_save3.do_package_passing();
         fact_save3.do_work(i);
     }

    generate_simulation_turn_report(fact_save3,reportsimulation,rounds);
    generate_structure_report(fact_save3,reportsimulation2);

    reportsimulation.close();
    reportsimulation2.close();
    */

    Worker wrk1(1, 2, std::make_unique<PackageQueue>(PackageQueue(PackageQueueType::FIFO)));
    Worker wrk2(2, 1, std::make_unique<PackageQueue>(PackageQueue(PackageQueueType::LIFO)));
    Ramp rmp1(1, 1);
    rmp1.receiver_preferences_.add_receiver(&wrk1);
    rmp1.receiver_preferences_.add_receiver(&wrk2);

    auto prefs = rmp1.receiver_preferences_.get_preferences();
    for(auto el:prefs){
        std::cout<<el.first->get_id()<<std::endl;
    }

    return EXIT_SUCCESS;
}
