
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Graph.h"
#include "GraphViewTheme.h"

class NodeComponent : public Component, public Graph::Node::Listener {
    
public:
    
    class PinComponent : public Component {
        
    public:
        
        NodeComponent* node;
        Graph::Pin* model;
        
        unsigned int cPinBackgroundCurrent;
        
        PinComponent(NodeComponent* node, Graph::Pin* model) : node(node), model(model) {
            cPinBackgroundCurrent =  node->theme.cPinBackground;
        }
        
        void paint(Graphics& g) {
            Path p;
            p.addRectangle(Rectangle<int>(0, 0, node->theme.pinWidth, node->theme.pinHeight));
            g.setColour(Colour(cPinBackgroundCurrent));
            g.fillPath(p);
        }
        
        
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PinComponent)
    };
    
    const GraphViewTheme theme;
    Graph::Node* model;
    
    
    OwnedArray<PinComponent> ins;
    OwnedArray<PinComponent> outs;
    
    bool selected;
    
    unsigned int cNodeBackgroundCurrent;
    
    float scaleFactor;
    AffineTransform translation;
    AffineTransform scale;
    
    
    NodeComponent(const GraphViewTheme& theme, Graph::Node* model);
    
    ~NodeComponent();
    
    bool hasIns();
    
    bool hasOuts();
    
    int boxHeight();
    
    Rectangle<int> boxBounds();
    
    virtual void paint(Graphics& g) override;
    
    virtual void resized() override;
    
    virtual bool hitTest(int x, int y) override;
    
    void mouseDown(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;
    void mouseMove(const MouseEvent& e) override;
    void mouseDoubleClick(const MouseEvent& e) override;
    void mouseExit(const MouseEvent& e) override;
    
private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NodeComponent)
    
};


























