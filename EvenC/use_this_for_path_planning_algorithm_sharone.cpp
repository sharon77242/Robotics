/*
 * use_this_for_path_planning_algorithm_sharone.cpp
 *
 *  Created on: Jul 31, 2017
 *      Author: user
 */

/////    link -  https://stackoverflow.com/questions/20600503/a-star-algorithm-get-optimal-path
//-------------------------------------------------------------------------------------------------
//class aNode
//{
//public:
//    QHash<quint64,Node>::iterator it;
//    quint64 comeFrom;
//    double f;
//    double g;
//    double h;
//}
//
//bool Graph::findPath(Node* node_from, Node* node_to)
//{
//    QMap<double,aNode> open; // key - cost function f
//    QMap<quint64,aNode> closed; // key - Node id
//
//    aNode sNode;
//    sNode.it = nodes.find(node_from->id);
//    sNode.g = 0;
//    sNode.h = distance(node_from,node_to);
//    sNode.f = sNode.g + sNode.h;
//    sNode.comeFrom = 0;
//
//    open.insert(sNode.f, sNode);
//
//    while (!open.isEmpty())
//    {
//        aNode x = open.begin().value();
//        if (x.it->id == node_to->id)
//        {
//            int comeFrom = x.comeFrom;
//            while (comeFrom != 0)
//            {
//                route.push_back(comeFrom);
//                comeFrom = closed.find(comeFrom)->comeFrom;
//            }
//            return true;
//        }
//        open.remove(x.f);
//        closed.insert(x.it->id,x);
//        for (int i = 0 ; i < x.it->adj.size() ; i++)
//        {
//            if (!edges.contains(QPair<quint64,quint64>(x.it->id,x.it->adj[i])))
//                continue;
//            if (closed.contains(x.it->adj[i]))
//                continue;
//            aNode y;
//            y.it = nodes.find(x.it->adj[i]);
//            y.g = x.g + edges.find(QPair<quint64,quint64>(x.it->id,x.it->adj[i]))->cost;
//            y.h = distance(&(*y.it),node_to);
//            y.f = y.g + y.h;
//            y.comeFrom = x.it->id;
//            if (open.key(y,-2) == -2)
//                open.insert(y.f,y);
//            else
//                if (y.g < open.find(open.key(y))->g )
//                    open.insert(y.f, y);
//        }
//    }
//    return false;
//}
//
//
