/*
    SPDX-FileCopyrightText: 2014-2017 Milian Wolff <mail@milianw.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef TRACETREE_H
#define TRACETREE_H

/**
 * @file tracetree.h
 * @brief Efficiently combine and store the data of multiple Traces.
 */

#include <algorithm>
#include <vector>

#include "trace.h"

struct TraceEdge
{
    Trace::ip_t instructionPointer;
    // index associated to the backtrace up to this instruction pointer
    // the evaluation process can then reverse-map the index to the parent ip
    // to rebuild the backtrace from the bottom-up
    uint32_t index;
    // sorted list of children, assumed to be small
    std::vector<TraceEdge> children;
};

/**
 * Top-down tree of backtrace instruction pointers.
 *
 * This is supposed to be a memory efficient storage of all instruction pointers
 * ever encountered in any backtrace.
 */
class TraceTree
{
public:
    void clear()
    {
        m_root.children.clear();
        m_index = 1;
    }

    /**
     * Index the data in @p trace and return the index of the last instruction
     * pointer.
     *
     * Unknown instruction pointers will be handled by the @p callback
     */
    template <typename Fun>
    uint32_t index(const Trace& trace, Fun callback)
    {
        uint32_t index = 0;
        TraceEdge* parent = &m_root;
        for (int i = trace.size() - 1; i >= 0; --i) {
            const auto ip = trace[i];
            if (!ip) {
                continue;
            }
            auto it =
                std::lower_bound(parent->children.begin(), parent->children.end(), ip,
                                 [](const TraceEdge& l, const Trace::ip_t ip) { return l.instructionPointer < ip; });
            if (it == parent->children.end() || it->instructionPointer != ip) {
                index = m_index++;
                it = parent->children.insert(it, {ip, index, {}});
                if (!callback(reinterpret_cast<uintptr_t>(ip), parent->index)) {
                    return 0;
                }
            }
            index = it->index;
            parent = &(*it);
        }
        return index;
    }

private:
    TraceEdge m_root = {0, 0, {}};
    uint32_t m_index = 1;
};

#endif // TRACETREE_H
