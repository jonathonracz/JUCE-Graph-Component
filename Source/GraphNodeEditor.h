#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Graph.h"

#if JUCE_MSVC
#pragma warning(push)
#pragma warning (disable : 4100)
#endif

class GraphNodeEditor {
public:
    
    virtual ~GraphNodeEditor() {}
    
    virtual Component* guest() = 0;
    
    virtual void setModel(Graph::Node* model) {}
    
    virtual void onData(const Graph::Node* sourceNode, const Graph::Pin* sourcePin, const var& data) {}
    
};

#if JUCE_MSVC
#pragma warning(pop)
#endif
