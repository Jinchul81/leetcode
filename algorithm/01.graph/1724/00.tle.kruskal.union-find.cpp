class DistanceLimitedPathsExist {
    class UnionFind {
    public:
      UnionFind(int n) : ids_(n), sz_(n, 1) {
        for (int i = 0; i < n; ++i) {
          ids_[i] = i;
        }
      }

      int find(int p) {
        if (ids_[p] == p) return p;
        return ids_[p] = find(ids_[p]);
      }

      void merge(int p, int q) {
        p = find(p), q = find(q);

        if (p == q) return;
        if (sz_[p] < sz_[q]) {
          sz_[q] += sz_[p];
          ids_[p] = q;
        } else {
          sz_[p] += sz_[q];
          ids_[q] = p;
        }
      }

    private:
      vector<int> ids_;
      vector<int> sz_;
    };

public:
    DistanceLimitedPathsExist(int n, vector<vector<int>>& edgeList)
      : uf_(n), n_(n), edges_(n), max_edges_(n) {
      auto comp = [](const auto& l, const auto& r) {
        return l[2] > r[2];
      };
      priority_queue<vector<int>, vector<vector<int>>, decltype(comp)> min_heap(comp);
      for (auto& edge : edgeList) {
        min_heap.push(edge);
      }
      while (min_heap.size()) {
          auto& curr = min_heap.top();
          auto u = curr[0], v = curr[1], w = curr[2];
          min_heap.pop();
          if (uf_.find(u) == uf_.find(v)) continue;
          uf_.merge(u, v);
          //cout << "u:" << u << " v:" << v << " w:" << w << endl;
          edges_[u][v] = w;
          edges_[v][u] = w;
      }
      vector<bool> visited(n, false);
      vector<int> parents;
      function<void(int)> dfs = [&](int u) {
        if (visited[u]) return;
        visited[u] = true;
        int max_cost = 0, child = u;
        for (auto itr = parents.rbegin(); itr != parents.rend(); ++itr) {
          int parent = *itr;
          max_cost = max(edges_[parent][child], max_cost);
          //out << "p:" << parent << " u:" << u << " mc:" << max_cost << endl;
          child = parent;
          max_edges_[parent][u] = max_cost;          
        }
        for (auto& [v, w] : edges_[u]) {
          parents.push_back(u);
          dfs(v);
          parents.pop_back();
        }
        return;
      };
      for (int u = 0; u < n; ++u) {
        dfs(u);
      }
    }
    
    bool query(int p, int q, int limit) {
      if (max_edges_[p].count(q)) {
        return max_edges_[p][q] < limit;
      } else if (max_edges_[q].count(p)) {
        return max_edges_[q][p] < limit;
      } else {
        if (uf_.find(p) == uf_.find(q)) {
          queue<pair<int, int>> que;
          vector<bool> visited(n_, false);
          que.push({p, 0});
          visited[p] = true;        
          while (que.size()) {
            auto [u, cost] = que.front(); que.pop();
            //cout << "u:" << u << " cost:" << cost << endl;
            if (u == q) return cost < limit;
            for (auto [v, w] : edges_[u]) {
              if (visited[v]) continue;
              visited[v] = true;
              que.push({v, max(cost, w)});
            }
          }   
        }
      }
      return false;
    }
private:
    int n_;
    UnionFind uf_;
    vector<unordered_map<int, int>> edges_;
    vector<unordered_map<int, int>> max_edges_;
};

/**
 * Your DistanceLimitedPathsExist object will be instantiated and called as such:
 * DistanceLimitedPathsExist* obj = new DistanceLimitedPathsExist(n, edgeList);
 * bool param_1 = obj->query(p,q,limit);
 */
