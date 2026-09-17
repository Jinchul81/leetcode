class DistanceLimitedPathsExist {
private:
  // Stores a value for each index at selected versions.
  //
  // For each index:
  //   versions_[index][i] is the version at which
  //   values_[index][i] became effective.
  //
  // The value remains valid until the next version entry.
  class VersionedArray {
  public:
    explicit VersionedArray(int size)
      : current_version_(0),
      values_(size),
      versions_(size) {
        for (int index = 0; index < size; ++index) {
          versions_[index].push_back(0);
          values_[index].push_back(-1);
        }
      }

    void set(int index, int value) {
      if (versions_[index].back() < current_version_) {
        versions_[index].push_back(current_version_);
        values_[index].push_back(value);
      } else {
        values_[index].back() = value;
      }
    }

    int get(int index, int version = -1) const {
      const auto& versions = versions_[index];
      if (version < 0) version = current_version_;
      const auto it = upper_bound(
          versions.begin(), versions.end(), version);

      const auto version_index =
        it - versions.begin() - 1;

      return values_[index][version_index];
    }

    void set_version(int version) {
      current_version_ = version;
    }

  private:
    int current_version_;
    vector<vector<int>> values_;
    vector<vector<int>> versions_;
  };

  class VersionedUnionFind {
  public:
    explicit VersionedUnionFind(int size)
      : data_(size) {}

    int find(int node, int version = -1) const {
      const int parent = data_.get(node, version);

      if (parent < 0) {
        return node;
      }

      return find(parent, version);
    }

    void unite(int x, int y) {
      int root_x = find(x);
      int root_y = find(y);

      if (root_x == root_y) {
        return;
      }

      int size_x = -data_.get(root_x);
      int size_y = -data_.get(root_y);

      // Keep root_x as the root of the larger component.
      if (size_x < size_y) {
        swap(root_x, root_y);
        swap(size_x, size_y);
      }

      data_.set(root_x, -(size_x + size_y));
      data_.set(root_y, root_x);
    }

    void set_version(int version) {
      data_.set_version(version);
    }

  private:
    VersionedArray data_;
  };

public:
  DistanceLimitedPathsExist(
      int n,
      vector<vector<int>>& edgeList)
    : union_find_(n) {

      sort(edgeList.begin(), edgeList.end(),
          [](const auto& lhs, const auto& rhs) {
          return lhs[2] < rhs[2];
          });

      for (int i = 0; i < edgeList.size();) {
        const int weight = edgeList[i][2];

        union_find_.set_version(weight);

        while (i < edgeList.size() &&
            edgeList[i][2] == weight) {
          union_find_.unite(
              edgeList[i][0],
              edgeList[i][1]);
          ++i;
        }
      }
    }

  bool query(int p, int q, int limit) const {
    const int version = limit - 1;

    return union_find_.find(p, version) ==
      union_find_.find(q, version);
  }

private:
  VersionedUnionFind union_find_;
};
