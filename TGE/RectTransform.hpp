#pragma once
#include "Utilities.hpp"

class RectTransform
{
public:

	RectTransform* GetParent() const;
	Rect GetWorldRect() const;
	Anchor GetAnchor() const;
	void SetAnchor( Anchor a_Anchor );
	Pivot GetPivot() const;
	void SetPivot( Pivot a_Pivot );
	Offset GetOffset() const;
	void SetOffset( Offset a_Offset );
	Size GetSize() const;
	void SetSize( Size a_Size );

private:

	RectTransform* m_Parent;
	Anchor m_Anchor;
	Pivot m_Pivot;
	Offset m_Offset;
	Size m_Size;
};