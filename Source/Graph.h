#pragma once


#include <memory>
#include <string>
#include <vector>
#include <functional>

#if JUCE_MSVC
#pragma warning(push)
#pragma warning (disable : 4100)
#endif

struct Graph {
    
    enum class PinType;
    struct Pin;
    struct Node;
    struct Edge;
    
    OwnedArray<Node> nodes;
    OwnedArray<Edge> edges;
    
    enum class PinType { In, Out };
    
    struct Pin {
        
        const PinType pinType;
        const int order;
        Node* node;
        
        Pin(const PinType& pinType, Node* node, const int order) : pinType(pinType), node(node), order(order) {
            
        }
        
        void flow(const var& data) {
            if (pinType == PinType::In) {
                //printf("[flow] in-pin:%d data:%s\n", order, data.stringValue.c_str());
                node->flow(this, data);
            } else {
                //printf("[flow] out-pin:%d data:%s\n", order, data.stringValue.c_str());
                std::vector<Pin*> targets = node->graph->targets(this);
                for (auto& p : targets) p->flow(data);
            }
        }
    };
    
    struct Node {
        
        struct Listener {
            virtual ~Listener() {}
            virtual void onData(const Node* sourceNode, const Pin* sourcePin, const var& data) {}
        };

        const Graph* graph;
        String name;
        OwnedArray<Pin> ins;
        OwnedArray<Pin> outs;
        ListenerList<Listener> listeners;
        
        Node(const Graph* graph, String name, int numIns, int numOuts) : graph(graph), name(name) {
            for (auto i = 0; i < numIns; i++) {
                ins.add(new Pin(PinType::In, this, i));
            }
            
            for (auto i = 0; i < numOuts; i++) {
                outs.add(new Pin(PinType::Out, this, i));
            }
        }
        
        void addListener(Listener* listener) {
            listeners.add(listener);
        }
        
        void removeListener(Listener* listener) {
            listeners.remove(listener);
        }
        
        void flow(const Pin* source, const var& data) {
            
            listeners.call(&Listener::onData, this, source, data);
            
            //publish(data);
            
        }
        
        void publish(const var& data) const {
            //printf("[publish] node:%s data:%s\n", name.c_str(), data.stringValue.c_str());
            for (auto& p : outs) {
                p->flow(data);
            }
        }
    };
    
    struct Edge {
        
        Pin* source;
        Pin* target;
        
        Edge(Pin* source, Pin* target) : source(source), target(target) {
            
        }
    };
    
    Node* addNode(String name, int numIns, int numOuts) {
        //printf("[graph-add-node] name:%s, ins: %d, outs:%d\n", name.c_str(), numIns, numOuts);
        nodes.add(new Node(this, name, numIns, numOuts));
        return nodes.getLast();
    }
    
    void removeNode(const Node* node) {
        //printf("[graph-remove-node] name:%s\n", node->name.c_str());
        // remove all connected edges
        std::vector<Edge*> edgesToRemove;
        for (auto& e : edges) {
            if (e->source->node == node || e->target->node == node) {
                edgesToRemove.push_back(e);
            }
        }
        for (auto& e : edgesToRemove) {
            removeEdge(e);
        }
        
        // remove the node
        nodes.removeObject(node, true);
        //report();
    }
    
    const Edge* addEdge(Pin* source, Pin* target) {
        //printf("[graph-add-edge] source-node:%s source-pin:%d, target-node:%s, target-pin:%d\n",
        //       source->node->name.c_str(), source->order, target->node->name.c_str(), target->order);
        edges.add(new Edge(source, target));
        return edges.getLast();
    }
    
    void removeEdge(const Edge* edge) {
        //printf("[graph-remove-edge] source-node:%s source-pin:%d, target-node:%s, target-pin:%d\n",
        //       edge->source->node->name.c_str(),
        //       edge->source->order,
        //       edge->target->node->name.c_str(),
        //       edge->target->order);
        edges.removeObject(edge, false);
        //report();
    }
    
    std::vector<Pin*> targets(const Pin* source) const {
        std::vector<Pin*> ret;
        for (auto& e : edges) {
            if (e->source == source) ret.push_back(e->target);
        }
        
        return ret;
    }
    
    std::vector<Node*> targets(const Node* source) const {
        std::vector<Node*> ret;
        for (auto& e : edges) {
            if (e->source->node == source) ret.push_back(e->target->node);
        }
        
        return ret;
    }
    
    void dfs(const Node* node, const std::function<void (const Node*)>& visit) const {
        std::vector<Node*> nodeTargets = targets(node);
        for (auto& n : nodeTargets) dfs(n, visit);
        visit(node);
    }
    
    void bfs(const Node* node, const std::function<void (const Node*)>& visit) const {
        visit(node);
        std::vector<Node*> nodeTargets = targets(node);
        for (auto& n : nodeTargets) bfs(n, visit);
    }
    
    void report() const {
        printf("#nodes:%lu, #edges:%lu\n", nodes.size(), edges.size());
    }
    
    
};

#if JUCE_MSVC
#pragma warning(pop)
#endif
