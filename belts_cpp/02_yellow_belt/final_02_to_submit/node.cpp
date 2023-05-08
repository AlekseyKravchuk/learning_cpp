#include "node.h"

#include "date.h"

EmptyNode::EmptyNode() noexcept {}

bool EmptyNode::Evaluate(const Date& date, const std::string& event) const noexcept {
    return true;
}

DateComparisonNode::DateComparisonNode(const Comparison& cmp_type, const Date& date) noexcept
    : _cmp_type(cmp_type), _date(date) {}

bool DateComparisonNode::Evaluate(const Date& date, const std::string& event) const noexcept {
    switch (_cmp_type) {
        case Comparison::Less:
            return date < _date;
        case Comparison::LessOrEqual:
            return date <= _date;
        case Comparison::Greater:
            return date > _date;
        case Comparison::GreaterOrEqual:
            return date >= _date;
        case Comparison::Equal:
            return date == _date;
        case Comparison::NotEqual:
            return date != _date;
        default:
            return false;
    }
}

EventComparisonNode::EventComparisonNode(const Comparison& cmp_type, const std::string& event)
    : _cmp_type(cmp_type), _event(event) {}

bool EventComparisonNode::Evaluate(const Date& date, const std::string& event) const noexcept {
    switch (_cmp_type) {
        case Comparison::Less:
            return event < _event;
        case Comparison::LessOrEqual:
            return event <= _event;
        case Comparison::Greater:
            return event > _event;
        case Comparison::GreaterOrEqual:
            return event >= _event;
        case Comparison::Equal:
            return event == _event;
        case Comparison::NotEqual:
            return event != _event;
        default:
            return 0;
    }
}

LogicalOperationNode::LogicalOperationNode(const LogicalOperation& logic_operation,
                                           const std::shared_ptr<Node>& lhs,
                                           const std::shared_ptr<Node>& rhs) noexcept
    : _logic_operation(logic_operation),
      _lhs_node(lhs),
      _rhs_node(rhs) {}

bool LogicalOperationNode::Evaluate(const Date& date, const std::string& event) const {
    switch (_logic_operation) {
        case LogicalOperation::Or:
            return _lhs_node->Evaluate(date, event) || _rhs_node->Evaluate(date, event);
        case LogicalOperation::And:
            return _lhs_node->Evaluate(date, event) && _rhs_node->Evaluate(date, event);
        default:
            return 0;
            break;
    }
}
