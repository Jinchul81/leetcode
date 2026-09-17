class DistanceLimitedPathsExist {
public:
    class Snapshot {
    public:
      Snapshot(int n) : curr_ts_(0), ids_(n), version_to_idx_(n) {
        for (int i = 0; i < n; ++i) {
          ids_[i].push_back(-1);
          version_to_idx_[i].push_back(-1);
        }
      }

      void set(int i, int v) {
        if (version_to_idx_[i].back() < curr_ts_)
          version_to_idx_[i].push_back(curr_ts_);

        if (ids_[i].size() < version_to_idx_[i].size())
          ids_[i].push_back(v);
        else
          ids_[i].back() = v;
      }

      int get(int i, int ts) {
        int ver = upper_bound(version_to_idx_[i].begin(), version_to_idx_[i].end(), ts) - version_to_idx_[i].begin() - 1;
        return ids_[i][ver];
      }

      int get(int i) {
        return get(i, curr_ts_);
      }

      void create_snapshot(int ts) {
        curr_ts_ = ts;
      }

    private:
      int curr_ts_;
      vector<vector<int>> ids_;
      vector<vector<int>> version_to_idx_;
    };

    class SnapshotUnionFind {
    public:
      SnapshotUnionFind(int n) : ss_(n) {
      }

      int find(int p, int ts) {
        int parent = ss_.get(p, ts);
        if (parent < 0) return p;
        return find(parent, ts);
      }

      int find(int p) {
        int parent = ss_.get(p);
        if (parent < 0) return p;
        return find(parent);
      }

      void merge(int p, int q) {
        int root_p = find(p), root_q = find(q);

        if (root_p == root_q) return;
        int sz_p = -ss_.get(root_p);
        int sz_q = -ss_.get(root_q);

        if (sz_p <= sz_q) {
          ss_.set(root_q, -sz_p-sz_q);
          ss_.set(root_p, root_q);
        } else {
          ss_.set(root_p, -sz_p-sz_q);
          ss_.set(root_q, root_p);
        }
      }

      void create_snapshot(int ts) {
        ss_.create_snapshot(ts);
      }

    private:
      Snapshot ss_;
    };

    DistanceLimitedPathsExist(int n, vector<vector<int>>& edgeList)
      : suf_(n) {
      sort(edgeList.begin(), edgeList.end(), [](const auto& l, const auto& r) {
        return l[2] < r[2];
      });

      int curr_weight = 0;
      for (int i = 0; i < edgeList.size();) {
        curr_weight = edgeList[i][2];
        suf_.create_snapshot(curr_weight);
        for (; i < edgeList.size() && curr_weight == edgeList[i][2]; ++i) {
          suf_.merge(edgeList[i][0], edgeList[i][1]);
        }
      }
    }
    
    bool query(int p, int q, int limit) {
      return suf_.find(p, limit - 1) == suf_.find(q, limit - 1);
    }

  private:
    SnapshotUnionFind suf_;
};

/**
 * Your DistanceLimitedPathsExist object will be instantiated and called as such:
 * DistanceLimitedPathsExist* obj = new DistanceLimitedPathsExist(n, edgeList);
 * bool param_1 = obj->query(p,q,limit);
 */
