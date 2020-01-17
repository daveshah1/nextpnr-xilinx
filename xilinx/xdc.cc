/*
 *  nextpnr -- Next Generation Place and Route
 *
 *  Copyright (C) 2019  David Shah <david@symbioticeda.com>
 *
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include "log.h"
#include "nextpnr.h"
NEXTPNR_NAMESPACE_BEGIN

void Arch::parseXdc(std::istream &in)
{

    if (!in)
        log_error("failed to open LPF file\n");
    std::string line;
    std::string linebuf;
    int lineno = 0;

    auto isempty = [](const std::string &str) {
        return std::all_of(str.begin(), str.end(), [](char c) { return isblank(c) || c == '\r' || c == '\n'; });
    };
    auto strip_quotes = [](const std::string &str) {
        if (str.at(0) == '"') {
            NPNR_ASSERT(str.back() == '"');
            return str.substr(1, str.size() - 2);
        } else if (str.at(0) == '{') {
            NPNR_ASSERT(str.back() == '}');
            return str.substr(1, str.size() - 2);
        } else {
            return str;
        }
    };
    auto split_to_args = [](const std::string &str, bool group_brackets) {
        std::vector<std::string> split_args;
        std::string buffer;
        auto flush = [&]() {
            if (!buffer.empty())
                split_args.push_back(buffer);
            buffer.clear();
        };
        int brcount = 0;
        for (char c : str) {
            if (c == '[' && group_brackets) {
                ++brcount;
            }
            if (c == ']' && group_brackets) {
                --brcount;
                buffer += c;
                if (brcount == 0)
                    flush();
                continue;
            }
            if (std::isblank(c)) {
                if (brcount == 0) {
                    flush();
                    continue;
                }
            }
            buffer += c;
        }
        flush();
        return split_args;
    };

    auto get_cells = [&](std::string str) {
        std::vector<CellInfo *> tgt_cells;
        if (str.empty() || str.front() != '[')
            log_error("failed to parse target (on line %d)\n", lineno);
        str = str.substr(1, str.size() - 2);
        auto split = split_to_args(str, false);
        if (split.size() < 2)
            log_error("failed to parse target (on line %d)\n", lineno);
        if (split.front() != "get_ports")
            log_error("targets other than 'get_ports' are not supported (on line %d)\n", lineno);
        IdString cellname = id(strip_quotes(split.at(1)));
        if (cells.count(cellname))
            tgt_cells.push_back(cells.at(cellname).get());
        return tgt_cells;
    };

    while (std::getline(in, line)) {
        ++lineno;
        // Trim comments, from # until end of the line
        size_t cstart = line.find('#');
        if (cstart != std::string::npos)
            line = line.substr(0, cstart);
        if (isempty(line))
            continue;

        std::vector<std::string> arguments = split_to_args(line, true);
        if (arguments.empty())
            continue;
        std::string &cmd = arguments.front();
        if (cmd == "set_property") {
            if (arguments.size() != 4)
                log_error("expected four arguments to 'set_property' (on line %d)\n", lineno);
            if (arguments.at(1) == "INTERNAL_VREF")
                continue;
            std::vector<CellInfo *> dest = get_cells(arguments.at(3));
            for (auto c : dest)
                c->attrs[id(arguments.at(1))] = std::string(arguments.at(2));
        } else {
            log_info("ignoring unsupported LPF command '%s' (on line %d)\n", cmd.c_str(), lineno);
        }
    }
    if (!isempty(linebuf))
        log_error("unexpected end of LPF file\n");
}

NEXTPNR_NAMESPACE_END
