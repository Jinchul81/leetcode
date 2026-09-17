class DistanceLimitedPathsExist {
public:
    class Snapshot {
    public:
      Snapshot(int n) : ts_(0), data_(n) {
        for (int i = 0; i < n; ++i) {
          data_[i][ts_] = 0;
        }
      }

      void set(int index, int value) {
        data_[index][ts_] = value;
      }

      int get(int index, int ts) {
        const auto& curr = data_[index];
        return prev(curr.upper_bound(ts))->second;
      }

      void snap(int ts) {
#if 0
        cout << "ts: " << ts_ << endl;
        for (auto& m : data_) {
          for (auto& e : m) {
            cout << e.first << ":" << e.second << " ";
          }
          cout << endl;
        }
#endif
        ts_ = ts;
      }

    protected:
      int ts_;
      vector<map<int, int>> data_;
    };

    class SnapshotUnionFind : public Snapshot {
    public:
      SnapshotUnionFind(int n) : Snapshot(n) {
        for (int i = 0; i < n; ++i) {
          set(i, -1);
        }
      }

      int find(int p, int ts) {
        int root_p = get(p, ts);
        if (root_p < 0) return p;
        return find(root_p, ts);
      }

      void merge(int u, int v) {
        int root_u = find(u, ts_);
        int root_v = find(v, ts_);
        if (root_u == root_v) return;

        int sz_u = -get(root_u, ts_);
        int sz_v = -get(root_v, ts_);
        if (sz_u <= sz_v) {
          set(root_v, - sz_u - sz_v);
          set(root_u, root_v);
        } else {
          set(root_u, - sz_u - sz_v);
          set(root_v, root_u);
        }
      }
    };

    DistanceLimitedPathsExist(int n, vector<vector<int>>& edgeList) : suf_(n) {
      sort(edgeList.begin(), edgeList.end(), [](const auto& l, const auto& r) {
        return l[2] < r[2];
      });

      int curr_weight = 0;
      for (int i = 0; i < edgeList.size();) {
        int curr_weight = edgeList[i][2];
        suf_.snap(curr_weight);

        while (i < edgeList.size() && curr_weight == edgeList[i][2]) {
          suf_.merge(edgeList[i][0], edgeList[i][1]);
          ++i;
        }
      }
      suf_.snap(curr_weight+1);

    }
    
    bool query(int p, int q, int limit) {
      return suf_.find(p, limit-1) == suf_.find(q, limit-1);
    }
    
    private:
    SnapshotUnionFind suf_;
};
