/*
  ==============================================================================

    Arrow.h
    Created: 30 Oct 2020 12:09:41pm
    Author:  syfo_dias

  ==============================================================================
*/

/** WHAT DO I WANT
    - object that takes a set of points and draws an arrow around the last point
    - object that takes a set of points and draws an arrow around the first and around the last point (twoSided)
 - 4 types of arrow: closed triangle, opened triangle, filled triangle, filled with sharp back edges
 - arrow length can't be longer than a line length
 - arrow can't be longer than a distance between the penultimate and the last point
 - linear, quadratic, and cubic lines must be supported
 
 Alternatively make a path outside, pass it to this class's object and add an arrow.
 
 SEQUENCE:
 - make lines + different types of arrowheads work
 - add possibility of curved lines
 
 Replace this later with Arrowhead class. Path will be drawn directly in component.
 Pass the path to the arrowhead class with args: g, path, angle, width, length.
 Initialise the arrowhead with view and bool fill
 Class curve?
 
 Explore how the curves can be drawn fast. 
 */

#pragma once

/** Arrow class prototype. */
/** Rebuild it to Curve. */
/** Rebuld this to arrowhead class, that will accept an arrow dimentions,
 colour, shape, and direction and attach it to the path passed from somewhere else
 (no need to draw a line or a curve here, just an arrowhead of custom shape. */
template <typename T>
class Arrow
{
public:
    Arrow() {}
    Arrow (const juce::Array<juce::Point<T>>& pt)
    :linePoints {pt}
    {}
    
    ~Arrow() {}
    
    enum class ArrowView { Tri, TriOpen, TriWithArc, FourPointer };
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
    
    void draw (juce::Graphics& g, const juce::Colour& colour, const float& thickness, const float& arrowheadWidth, const float& arrowheadLength)
    {
        const auto numPoints = linePoints.size();
        if (numPoints < 2)
            return;
        if (numPoints > 2)
        {
            juce::Path p;
            p.startNewSubPath (linePoints.getUnchecked (0));
            for (int i = 1; i < numPoints - 1; ++i)
            {
                p.lineTo (linePoints.getUnchecked (i));
            }
            g.setColour (colour);
            g.strokePath (p, juce::PathStrokeType {thickness});
        }
        
        auto lastLine = juce::Line<T> { linePoints.getUnchecked (numPoints - 2), linePoints.getUnchecked (numPoints - 1) };
        auto arrowLength = juce::jmin ( lastLine.getLength() * 0.8f, arrowheadLength);
        drawArrowhead(g, lastLine, colour, thickness, arrowheadWidth, arrowLength);

    }
    
    void draw (juce::Graphics& g, juce::Path& p, const juce::Colour& colour, const float& thickness, const float& arrowheadWidth, const float& arrowheadLength)
    {
        
    }
    
    
    void drawTwoSidedArrow (juce::Graphics& g, const juce::Colour& colour, const float& thickness, const float& arrowheadWidth, const float& arrowheadLength)
    {

    }
    
    void setFill (const bool fill)
    {
        isFilled = fill;
    }
    
private:
    juce::Array<juce::Point<T>> linePoints;
    ArrowView arrowPointerView { ArrowView::Tri};
    bool isFilled { false };
    /** TODO:
     - determine the angle of the arrow (direction). What if the line is curved? Differential?
     - use affine transform (see draw pointer as triangle for slider)
     - other types of arrows  (later)
     */
    void drawArrowhead (juce::Graphics& g, const juce::Line<float>& line, const juce::Colour& colour, float lineThickness, float arrowheadWidth, float arrowheadLength) noexcept
    {
        /* FILLED */
        
        /* EMPTY CLOSED */
        if (arrowPointerView == ArrowView::Tri)
        {
            auto reversed = line.reversed();
            arrowheadWidth *= 0.5f;
            //lineThickness *= 0.5f;
            juce::Path arrow;
            arrow.startNewSubPath (line.getPointAlongLine(0.0f, 0.0f));
            arrow.lineTo (reversed.getPointAlongLine(arrowheadLength, 0.0f));
            arrow.closeSubPath();
            g.setColour (colour);
            g.strokePath (arrow, juce::PathStrokeType {lineThickness});
            //arrow.lineTo (line.getPointAlongLine (0, -lineThickness));
            juce::Path arrowhead;
            arrowhead.startNewSubPath (reversed.getPointAlongLine (arrowheadLength, 0.0f));
            arrowhead.lineTo (reversed.getPointAlongLine (arrowheadLength, arrowheadWidth));
            arrowhead.lineTo (line.getEnd());
            arrowhead.lineTo (reversed.getPointAlongLine (arrowheadLength, -arrowheadWidth));
            arrowhead.lineTo (reversed.getPointAlongLine (arrowheadLength, 0.0f));
            arrowhead.closeSubPath();
            if (isFilled)
            {
                g.fillPath (arrowhead);
            }
            else
            {
                /** Empty. */
                g.strokePath (arrowhead, juce::PathStrokeType {lineThickness});
            }
        }
        
        /* EMPTY OPENED */
        
    };
    
    void drawArrowhead (juce::Graphics& g, const juce::Colour& colour, const float& lineThickness, const float& width, const float& length, const float& tipX, const float& tipY, const float& direction) noexcept
    {
        /* FILLED */
        
        /* EMPTY CLOSED */
        if (arrowPointerView == ArrowView::Tri)
        {
            if (isFilled)
            {
                
            }
            else
            {
                /** Empty. */
            }
        }
        
        /* EMPTY OPENED */ 
  
    }
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Arrow)

};

