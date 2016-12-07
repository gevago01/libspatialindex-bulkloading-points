//
// Created by giannis on 01/11/16.
//

#ifndef BULKLOADRTREE_MYDATASTREAM_H
#define BULKLOADRTREE_MYDATASTREAM_H

#include "Point.h"

class MyDataStream : public SpatialIndex::IDataStream {
private:
    std::set<uint32_t> random_indices;
    std::ifstream m_fin;
    int status;
    SpatialIndex::RTree::Data *m_pNext;
    long num_of_clusters;
    long cluster_size;
    long num_of_points;
    uint32_t dimensionality;
    std::vector<Point> random_points;
    long max_clusterid;
    uint32_t point_counter = 0;
    const uint32_t measurements;
public:
    const std::vector<Point> &getRandom_points() const {
        return random_points;
    }

    long getMax_clusterid() const {
        return max_clusterid;
    }

    void findRandomIndices() {
        auto n=0;
        if (num_of_points<measurements){
            n=num_of_points;
        }
        else{
            n=measurements;
        }
        srand(time(NULL));
        while (random_indices.size() != n) {
            int randNum = rand() % (num_of_points);
            random_indices.insert(randNum);
        }
    }


    void readHeaders() {
        if (!m_fin) {
            std::cout << "failed to read input" << std::endl;
            exit(-1);
        }
        std::string line, word;
        std::vector<std::string> tokens;
        bool num_of_points_done = false;
        while (getline(m_fin, line)) {

            if (num_of_points_done) {
                break;
            }
            //check comment lines
            if (line[0] == '#') {
                num_of_points_done = getFileStats(line);
                continue;
            }


        }
    }

    bool getFileStats(std::string line) {

        //if line contains the number of clusters
        if ((line.find("Number of clusters") != std::string::npos)) {
            std::stringstream file_info(line);
            std::string temp_str;
            // go to the last token
            while (file_info >> temp_str);
            //last token of line is the # of clusters
            num_of_clusters = std::stol(temp_str);
            std::cout << "Number of clusters:"<< num_of_clusters << std::endl;

            return false;
        }

        if ((line.find("Size") != std::string::npos)) {
            std::istringstream file_info(line);
            std::string temp_str;
            // go to the last token
            while (file_info >> temp_str){

            }
            //last token is cluster size
            cluster_size = std::stol(temp_str);
            std::cout << "Cluster size:" << cluster_size << std::endl;
            max_clusterid = num_of_clusters;
            num_of_points = num_of_clusters * cluster_size;
            std::cout << "Number of points:" << num_of_points << std::endl;
            return false;
        }

        if ((line.find("Normal") != std::string::npos)) {
            ++dimensionality;
            return false;
        }

        if ((line.find("Discards") != std::string::npos)) {
            std::cout << "Dimensionality:" << dimensionality << std::endl;
            return true;
        }
        return false;
    }

    MyDataStream(std::string inputFile, int numof_queries) : status(0), m_pNext(0), measurements(numof_queries) {
        max_clusterid = std::numeric_limits<long>::min();
        m_fin.open(inputFile);

        if (!m_fin)
            throw Tools::IllegalArgumentException("Input file not found.");
        //get required info from headers
        readHeaders();
        //usefull for finding random points
        findRandomIndices();
        readNextEntry();
    }

    virtual ~MyDataStream() {
        if (m_pNext != 0)
            delete m_pNext;
    }

    virtual SpatialIndex::IData *getNext() {

        if (m_pNext == 0)
            return 0;

        SpatialIndex::RTree::Data *ret = m_pNext;
        m_pNext = 0;
        readNextEntry();
        return ret;
    }


    void readNextEntry() {

        std::string line;
        //get next line in the file
        getline(m_fin, line);

        while (line[0] == '#') {
            //get next line in the file
            getline(m_fin, line);
        }


        if (m_fin.good()) {

            //istringstream makes the line string an input string stream
            std::istringstream record(line);
            std::string word;
            std::vector<std::string> tokens;
            /*a record contains a number of coordinates and a cluster id:
            2.58119273751133 -3.0897997256242977 1*/
            while (record >> word) {
                tokens.push_back(word);
            }

            Point point(tokens);
            //get random point
            if (random_indices.find(point_counter) != random_indices.end()) {
                random_points.push_back(point);
            }


            dimensionality = point.getCoordinates().size();
            double coords[dimensionality];
            int ic = 0;
            for (double d:point.getCoordinates()) {
                coords[ic] = d;
                ic++;
            }
            SpatialIndex::Point s_point(coords, dimensionality);
            //a region with low==high is a point - see
            //https://github.com/libspatialindex/libspatialindex/issues/69
            SpatialIndex::Region r(s_point, s_point);

            /***********have to store cluster id along with point***********/
            std::string string_cluster(std::to_string(point.getCluster_id()));
            auto len = string_cluster.length();
            char const *p = string_cluster.c_str();
            /***************************************************************/
            //constructor only takes regions
            m_pNext = new SpatialIndex::RTree::Data(len, (byte *) p, r, point.getPoint_id());
            // Once the data array is given to RTRee:Data a local copy will be created

            ++point_counter;
        }

    }

    virtual bool hasNext() {
        return (m_pNext != 0);
    }

    virtual uint32_t size() {
        throw Tools::NotSupportedException("Operation not supported.");
    }

    virtual void rewind() {
        if (m_pNext != 0) {
            delete m_pNext;
            m_pNext = 0;
        }

        m_fin.seekg(0, std::ios::beg);
        readNextEntry();
    }


    unsigned long getDimensionality() const {
        return dimensionality;
    }


};
#endif //BULKLOADRTREE_MYDATASTREAM_H
