#pragma once
#include "afx.h"

namespace OldGraphLabel {
#include "../CommonFiles/GraphLabel.h"
}

struct GraphLabel : public OldGraphLabel::GraphLabel {

	GraphLabel(GraphPoint &target, Color color) : OldGraphLabel::GraphLabel(target, color) {}
};
