//
// Created by giannis on 01/11/16.
//

#ifndef BULKLOADRTREE_MYVISITOR_H
#define BULKLOADRTREE_MYVISITOR_H
#include <spatialindex/SpatialIndex.h>

class MyVisitor : public SpatialIndex::IVisitor {

private:
    uint32_t data_length;
    std::string cluster_id;
    std::vector<size_t> _indices;

public:
    const std::string &getCluster_id() const {
        return cluster_id;
    }

    MyVisitor(uint32_t const max_cluster_id) {
        data_length = std::to_string(max_cluster_id).length();
    }

    void visitNode(const SpatialIndex::INode &) {
    }


    void visitData(std::vector<const SpatialIndex::IData *> &data) {
        std::cout<<"size:"<<data.size()<<std::endl;
    }

    void visitData(const SpatialIndex::IData &d) {
        //_indices.push_back(d.getIdentifier());

        byte *bytearray = 0;
        //plus one for the a null terminated string
        char char_cluster_id[data_length + 1];

        d.getData(data_length, &bytearray);
        std::copy(&bytearray[0], &bytearray[data_length], char_cluster_id);
        char_cluster_id[data_length] = '\0';

        cluster_id = char_cluster_id;

    }

    std::vector<size_t> &indices() { return _indices; }

    std::vector<double> getTimes();


};

#endif //BULKLOADRTREE_MYVISITOR_H
