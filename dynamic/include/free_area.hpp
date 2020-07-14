#pragma once

#include <vector>
#include <stack>
#include <tuple>
#include <unordered_set>

namespace free_area {

using row  = std::vector<int>;
using area = std::vector<row>;

// ----------------------------
struct point {
   point (int x, int y) : x(x), y(y) {}
   int x{0};
   int y{0};
};
bool operator == (const point& lhs, const point& rhs) {
   return lhs.x == rhs.x &&
          lhs.y == rhs.y;
}
using points = std::vector<point>;

} // namespace free_area

// inject hash<free_area::point> into std
namespace std {
template<> struct hash<free_area::point> {
   size_t operator() (const free_area::point& p) const noexcept {
      return std::hash<decltype(p.x)>{}(p.x) ^ (std::hash<decltype(p.y)>{}(p.y) << 1);
   }
};
} // namespace std;

namespace free_area {

// ------------------------------------------------------------------
//  {1,1},     0 0 0 0    1 1 1 1
//  {2,0}  =>  0 1 0 0 => 2 0 2 2
//             1 0 0 0    0 1 3 3
area column_squares(int h, int w, const points& ps) {
   area res(h, row(w));

   std::unordered_set<point> us;
   for (auto& p: ps) {
      us.emplace(p.x, p.y);
   }

   for (int i = 0; i < h; ++i) {
      for (int j = 0; j < w; ++j) {
         if (us.find(point{i, j}) == us.end()) {
            res[i][j] = i > 0 ? res[i-1][j] + 1 : 1;
         } else {
            res[i][j] = 0;
         }
      }
   }
   return res;
}

// ------------------------------------------------------------------
std::tuple<row, row> left_right_boards(const row& r) {
   const int sz = r.size();
   assert(sz > 0);

   row left(sz, 0);
   std::stack<int> s;
   s.push(sz-1);
   for (int i = sz-2; i >= 0; --i) {
      assert(!s.empty());
      int top = s.top();
      if (r[i] < r[top]) {
         while(!s.empty() && (r[s.top()] > r[i])) {
            left[s.top()] = top;
            s.pop();
         }
      }
      s.push(i);
   }
   std::stack<int> empty;
   empty.swap(s);

   row right(sz, sz-1);
   s.push(0);
   for (int i = 1; i < sz; ++i) {
      assert(!s.empty());
      int top = s.top();
      if (r[i] < r[top]) {
         while (!s.empty() && r[s.top()] > r[i] ) {
            right[s.top()] = top;
            s.pop();
         }
      }
      s.push(i);
   }
   return std::make_tuple(left, right);
}

// ------------------------------------------------------------------
//  0 0 0 0    4 3 2 1
//  0 1 0 0 => 1 0 2 1
//  1 0 0 0    0 3 2 1
area calc_area(const area& a) {
   assert(!a.empty());
   const int h = a.size();
   const int w = a.front().size();
   area res(h, row());

   for (int i = 0; i < h; ++i) {
      int cnt = 0;
      for (int j = w-1; j >= 0; --j) {
         if (a[i][j] == 1) {
            cnt = 0;
         } else {
            ++cnt;
         }
         res[i][j] = cnt;
      }
   }
   return res;
}

// ------------------------------------------------------------------
int max_square(const area& a) {
   const area ca = calc_area(a);

   assert(ca.size());
   const int h = ca.size();
   const int w = ca.front().size();

   int max_area = 0;
   for (int i = 0; i < h; ++i) {
      for (int j = 0; j < w; ++j) {

         int cur_w = INT32_MAX;
         int cur_h = 0;

         for(int k = i; k < h; ++k){
            if (cur_w > ca[k][j]) {
               cur_w = ca[k][j];
            }
            if (cur_w == 0) {
               break;
            }
            ++cur_h;

            if (max_area < cur_h*cur_w) {
               max_area = cur_h*cur_w;
            }
         }
      }
   }
   return max_area;
}

// ------------------------------------------------------------------
int find_max_square(const row& s, const row& l, const row& r) {
   const auto sz = s.size();
   assert(sz > 0);
   assert(sz == l.size() && sz == r.size());

   int res = -1;
   for (size_t i = 0; i < sz; ++i) {
      int tmp = (r[i] - l[i] + 1) * s[i];

      if (res < tmp) {
         res = tmp;
      }
   }

   return res;
}

// ------------------------------------------------------------------
int max_square_ex(int h, int w, const points& p) {
   const area& cs = column_squares(h, w, p);
   int max_square = -1;
   for (const auto& r: cs) {
      row left;
      row right;

      std::tie(left, right) = left_right_boards(r);
      int s = find_max_square(r, left, right);

      if (max_square < s) {
         max_square = s;
      }
   }

   return max_square;
}

} // namespace free_area
