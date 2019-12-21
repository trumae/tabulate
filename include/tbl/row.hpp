#pragma once
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <tbl/cell.hpp>
#include <vector>

namespace tbl {

class Row {
public:
  void add_cell(const Cell &cell) { cells_.push_back(cell); }

  Cell &operator[](size_t index) { return cells_[index]; }

  std::vector<Cell> cells() const { return cells_; }

  size_t size() const { return cells_.size(); }

  std::optional<Cell> get_cell(size_t index) const {
    if (index < cells_.size())
      return cells_[index];
  }

  size_t height() const {
    size_t result{1};
    for (auto &cell : cells_) {
      auto cell_data = cell.data_;
      if (cell.format_.has_value()) {
        auto format_width = cell.format_.value().width_;
        // If cell contents are wider than the
        // cell format.width, then the height of
        // the row haas to be incremented to
        // fit the cell contents
        //
        // e.g., if cell text = "ABCD" and format.width = 3
        // then, cell should show:
        // ABC
        // -D
        // So, the returned height = 2
        if (format_width.has_value() && cell_data.size() > format_width.value()) {
          result = std::max(result, cell_data.size() / format_width.value());
        } else {
          result = std::max(result, size_t(1));
        }
      }
    }
    return result;
  }

  Format &format() {
    if (!format_.has_value())
      format_ = table_format_;
    return format_.value();
  }

private:
  explicit Row(Format &table_format) : table_format_(table_format) {}

  friend class Table;
  std::vector<Cell> cells_;
  std::reference_wrapper<Format> table_format_;
  std::optional<Format> format_;
};

} // namespace tbl