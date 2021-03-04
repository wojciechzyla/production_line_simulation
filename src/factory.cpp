#include "factory.hpp"

void Factory::remove_worker(ElementID id) {
    remove_receiver(workers_,id);
    workers_.remove_by_id(id);
}

void Factory::remove_storehouse(ElementID id) {
    remove_receiver(storehouses_,id);
    storehouses_.remove_by_id(id);
}

void Factory::do_deliveries(Time t) {
    for (auto it = ramps_.begin(); it != ramps_.end(); it++){
        it->deliver_goods(t);
    }
}

void Factory::do_package_passing() {
    for (auto it = ramps_.begin(); it != ramps_.end(); it++){
        it->send_package();
    }
    for (auto it = workers_.begin(); it != workers_.end(); it++){
        it->send_package();
    }
}

void Factory::do_work(Time t) {
    for (auto it = workers_.begin(); it != workers_.end(); it++){
        it->do_work(t);
    }
}

template <class Node>
void Factory::remove_receiver(NodeCollection<Node>& collection, ElementID id) {
    auto elem = collection.find_by_id(id);
    for (auto it = workers_.begin(); it != workers_.end(); it++){
        it->receiver_preferences_.remove_receiver(&*elem);
    }
    for (auto it = ramps_.begin(); it != ramps_.end(); it++){
        it->receiver_preferences_.remove_receiver(&*elem);
    }
}

bool Factory::is_consistent() {
    std::map<PackageSender*, NodeColor> s_color;

    for(auto it=workers_.begin(); it!=workers_.end(); it++){
        s_color.emplace(std::make_pair(&*it, NodeColor::UNVISITED));
    }
    for(auto it=ramps_.begin(); it!=ramps_.end(); it++){
        s_color.emplace(std::make_pair(&*it, NodeColor::UNVISITED));
    }

    try {
        for(auto it=ramps_.begin(); it!=ramps_.end(); it++){
            has_reachable_storehouse(&*it,s_color);
        }
    }
    catch (std::logic_error& er) {
        return false;
    }
    return true;
}

bool Factory::has_reachable_storehouse(PackageSender *p_sender, std::map<PackageSender*, NodeColor> s_color) {
    if (s_color[p_sender] == NodeColor::VERIFIED){
        return true;
    }
    s_color[p_sender] = NodeColor::VISITED;

    if (p_sender->receiver_preferences_.begin() == p_sender->receiver_preferences_.end()){
        throw std::logic_error("Nadawca nie posiada żadnego połączenia wyjściowego. factory.cpp");;
    }

    bool more_receivers_than_self = false;

    for (auto it = p_sender->receiver_preferences_.begin(); it != p_sender->receiver_preferences_.end(); it++){
        if(it->first->get_receiver_type() == ReceiverType::STOREHOUSE){
            more_receivers_than_self = true;
        }
        else if(it->first->get_receiver_type() == ReceiverType::WORKER){
            IPackageReceiver* receiver_ptr = it->first;
            auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);

            if(sendrecv_ptr == p_sender){
                continue;
            }
            more_receivers_than_self = true;

            if(s_color[sendrecv_ptr] != NodeColor::VISITED){
                try {
                    has_reachable_storehouse(sendrecv_ptr, s_color);
                }
                catch (std::logic_error& er) {
                    throw std::logic_error("Nadawca nie posiada żadnego połączenia wyjściowego. factory.cpp");
                }
            }
        }
    }

    s_color[p_sender] = NodeColor::VERIFIED;
    if(more_receivers_than_self){
        return true;
    }
    else{
        throw std::logic_error("Nadawca nie posiada zadnego polaczenia wyjsciowego. factory.cpp");
    }

}

// File loading

ElementType string_to_element_type(std::string type_string){
    ElementType result;
    if (type_string == "LOADING_RAMP") result = ElementType::RAMP;
    else if (type_string == "WORKER") result = ElementType::WORKER;
    else if (type_string == "STOREHOUSE") result = ElementType::STOREHOUSE;
    else if (type_string == "LINK") result = ElementType::LINK;
    else {throw std::logic_error("Bledny typ elementu! factory.cpp");}
    return result;
}

PackageQueueType string_to_queue_type(std::string queue_string){
    if (queue_string == "FIFO") return PackageQueueType::FIFO;
    else if (queue_string == "LIFO") return PackageQueueType::LIFO;
    else throw std::logic_error("Bledny typ kolejki. factory.cpp");
}


void add_links(std::map<std::string, std::string> link_data, Factory* factory){
    std::string source = link_data["src"];
    std::string destination = link_data["dest"];
    std::vector<std::string> src_divided;
    std::vector<std::string> dest_divided;
    std::string src;
    std::string dest;
    char delimiter = '-';

    std::istringstream src_stream(source);
    while (std::getline(src_stream, src, delimiter)){
        src_divided.push_back(src);
    }

    std::istringstream dest_stream(destination);
    while (std::getline(dest_stream, dest, delimiter)){
        dest_divided.push_back(dest);
    }

    if (*src_divided.begin() == "ramp"){

        if (*dest_divided.begin() == "worker"){
            IPackageReceiver* rec = &*factory->find_worker_by_id(std::stoi(*(dest_divided.end()-1)));
            factory->find_ramp_by_id(std::stoi(*(src_divided.end()-1)))->receiver_preferences_.add_receiver(rec);
        } else if (*dest_divided.begin() == "store"){
            IPackageReceiver* rec = &*factory->find_storehouse_by_id(std::stoi(*(dest_divided.end()-1)));
            factory->find_ramp_by_id(std::stoi(*(src_divided.end()-1)))->receiver_preferences_.add_receiver(rec);
        }
        else throw std::logic_error("Bledne dane - src=ramp des!=worker!=store");

    } else if (*src_divided.begin() == "worker"){

        if (*dest_divided.begin() == "worker"){
            IPackageReceiver* rec = &*factory->find_worker_by_id(std::stoi(*(dest_divided.end()-1)));
            factory->find_worker_by_id(std::stoi(*(src_divided.end()-1)))->receiver_preferences_.add_receiver(rec);
        } else if (*dest_divided.begin() == "store") {
            IPackageReceiver* rec = &*factory->find_storehouse_by_id(std::stoi(*(dest_divided.end()-1)));
            factory->find_worker_by_id(std::stoi(*(src_divided.end()-1)))->receiver_preferences_.add_receiver(rec);
        } else throw std::logic_error("Bledne dane - src:worker des!=worker!=store");

    } else throw std::logic_error("Bledne dane - src!=ramp!=worker");
}

ParsedLineData parse_line(std::string line){
    std::vector<std::string> tokens;
    std::string token;
    ParsedLineData data;

    std::istringstream token_stream(line);
    char delimiter = ' ';
    while (std::getline(token_stream, token, delimiter)){
        tokens.push_back(token);
    }

    data.element_type_ = string_to_element_type(*tokens.begin());

    char token_delimiter = '=';
    std::string key_val;
    std::vector<std::string> keyval_vect;
    for (auto i = tokens.begin()+1; i != tokens.end(); i++){
        std::istringstream token_string(*i);
        while (std::getline(token_string, key_val, token_delimiter)){
            keyval_vect.push_back(key_val);
        }
        auto end = keyval_vect.end();
        end--;
        data.parameters_.emplace(std::make_pair(*keyval_vect.begin(), *end));
        keyval_vect.clear();
    }

    return data;
}

Factory load_factory_structure(std::istream& is){
    std::string line;
    Factory factory;
    while(std::getline(is, line)) {
        if ((line[0] == ';') or (line.empty())) {
            continue;
        }
        ParsedLineData tokens = parse_line(line);
        switch (tokens.element_type_){
            case ElementType::RAMP:
                factory.add_ramp(Ramp(std::stoi(tokens.parameters_["id"]), std::stoi(tokens.parameters_["delivery-interval"])));
                break;
            case ElementType::WORKER:
                factory.add_worker(Worker(std::stoi(tokens.parameters_["id"]), std::stoi(tokens.parameters_["processing-time"]), std::make_unique<PackageQueue>(PackageQueue(string_to_queue_type(tokens.parameters_["queue-type"])))));
                break;
            case ElementType::STOREHOUSE:
                factory.add_storehouse(Storehouse(std::stoi(tokens.parameters_["id"])));
                break;
            case ElementType::LINK:
                add_links(tokens.parameters_, &factory);
                break;
        }
    }
    return factory;
}

template <class Node>
std::vector<std::list<int>> sorted_workers_stores(Node source){
    std::list<int> workers;
    std::list<int> storehouses;
    for (auto it = source->receiver_preferences_.cbegin(); it != source->receiver_preferences_.cend(); it++) {
        if(it->first->get_receiver_type() == ReceiverType::WORKER) workers.emplace_back(it->first->get_id());
        else if(it->first->get_receiver_type() == ReceiverType::STOREHOUSE) storehouses.emplace_back(it->first->get_id());
        else throw std::logic_error("Niepoprawny receive type");
    }
    storehouses.sort();
    workers.sort();
    std::vector<std::list<int>> result = {storehouses,workers};
    return result;
}

template <class Node>
void save_links(std::ostream& os, Node source, std::string stc_type){

    std::vector<std::list<int>> heler_vector = sorted_workers_stores(source);

    for(auto s:heler_vector[0]){
        os << "LINK src="<< stc_type <<"-" << source->get_id() << " dest=" << "store" << "-" <<s<<"\n";
    }
    for(auto w:heler_vector[1]){
        os << "LINK src="<< stc_type <<"-" << source->get_id() << " dest=" << "worker" << "-" <<w<<"\n";
    }


    if(source->receiver_preferences_.cbegin() != source->receiver_preferences_.cend()){
        os<<"\n";
    }
}

void save_factory_structure(Factory& factory, std::ostream& os){
    os<<"; == LOADING RAMPS ==\n\n";
    for (auto it = factory.ramp_cbegin(); it != factory.ramp_cend(); it++){
        os<<"LOADING_RAMP id="<<it->get_id()<<" delivery-interval="<<it->get_delivery_interval()<<"\n";
    }

    os<<"\n; == WORKERS ==\n\n";
    for (auto it = factory.worker_cbegin(); it != factory.worker_cend(); it++){
        os<<"WORKER id="<<it->get_id()<<" processing-time="<<it->get_processing_duration()<<" queue-type="<<queue_type_to_string(it->get_queue()->get_queue_type())<<"\n";
    }

    os<<"\n; == STOREHOUSES ==\n\n";
    for (auto it = factory.storehouse_cbegin(); it != factory.storehouse_cend(); it++){
        os<<"STOREHOUSE id="<<it->get_id()<<"\n";
    }

    os<<"\n; == LINKS ==\n\n";
    for (auto it = factory.ramp_cbegin(); it != factory.ramp_cend(); it++){
        save_links(os, it, "ramp");
    }

    for (auto it = factory.worker_cbegin(); it != factory.worker_cend(); it++){
        save_links(os, it, "worker");
    }
}