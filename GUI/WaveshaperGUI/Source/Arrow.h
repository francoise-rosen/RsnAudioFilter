/*
  ==============================================================================

    Arrow.h
    Created: 30 Oct 2020 12:09:41pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

/** Arrow class prototype. */
template <typename T>
class Arrow
{
public:
    Arrow() {}
    Arrow (const juce::Array<juce::Point<T>>& pt)
    :linePoints {pt}
    {}
    
    ~Arrow() {}
    enum class ArrowView { ClosedEmpty, OpenedEmpty, Filled };
    enum class SegmentType { Line, Quadratic, Cubic };
    
    void setLinePoints (const juce::Array<juce::Point<T>>& pt)
    {
        linePoints.clear();
        linePoints.resize (pt.size());
        std::copy (pt.begin(), pt.end(), linePoints.begin());
    }
    
    void setLinePoints (const T& x1, const T& y1, const T& x2, const T& y2)
    {
        linePoints.clear();
        linePoints.add (juce::Point<T> {x1, y1});
        linePoints.add (juce::Point<T> {x2, y2});
    }
    
    void setLinePoints (const juce::Point<T>& p1, const juce::Point<T>& p2)
    {
        linePoints.clear();
        linePoints.add (p1);
        linePoints.add (p2);
    }
    
    juce::Path getPath()
    {
        juce::Path local;
        local.startNewSubPath (linePoints.getUnchecked(0));
        for (int i = 1; i < linePoints.size(); ++i)
        {
            local.lineTo (linePoints[i]);
        }
        local.closeSubPath();
        return local;
    }
    
    void draw (juce::Graphics& g, const juce::Colour& colour, const float& thickness, const float& arrowheadWidth, const float& arrowheadLength)
    {
        g.setColour (colour);
        juce::Path p = std::move (getPath());
        g.strokePath (p, juce::PathStrokeType {thickness});
        drawPointer(g, colour, thickness, arrowheadWidth, arrowheadLength, linePoints[linePoints.size()-1].getX(), linePoints[linePoints.size()-1].getY(), 1.0f);
    }
    
    // this will modify the ppathh that is passed!
    void addToPath (juce::Path& p)
    {
       if (p.isEmpty())
           return;
       if (linePoints.size() == 0)
       {
           
       }
    }
    
private:
    juce::Array<juce::Point<T>> linePoints;
    ArrowView arrowPointerView { ArrowView::ClosedEmpty};
    /** TODO:
     - determine the angle of the arrow (direction). What if the line is curved? Differential?
     - use affine transform (see draw pointer as triangle for slider)
     - other types of arrows  (later)
     */
    void drawPointer (juce::Graphics& g, const juce::Colour& colour, const float& lineThickness, const float& width, const float& length, const float& tipX, const float& tipY, const float& direction) noexcept
    {
        if (arrowPointerView == ArrowView::Filled
            || arrowPointerView == ArrowView::ClosedEmpty)
        {
            juce::Path p;
            p.startNewSubPath (tipX, tipY);
            p.lineTo (tipX - width * 0.5f, tipY);
            p.lineTo (tipX, tipY - length);
            p.lineTo (tipX + width * 0.5f, tipY);
            p.closeSubPath();
            p.applyTransform (juce::AffineTransform::rotation (juce::MathConstants<float>::halfPi * direction, tipX, tipY));
            g.setColour (colour);
            if (arrowPointerView == ArrowView::Filled)
                g.fillPath (p);
            else
                g.strokePath (p, juce::PathStrokeType {lineThickness});
        }
    }
    
    void drawPointer2 (juce::Graphics& g, const juce::Line<float> line, const float& lineThickness, const float& arrowheadWidth, const float& arrowheadHeight)
    {
        if (arrowPointerView == ArrowView::ClosedEmpty)
        {
            
        }
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Arrow)

};
