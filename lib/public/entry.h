namespace cash {

class EntryBase {
public:
  virtual ~EntryBase() = default;
};

template <typename KEY, typename VAL>
class Entry : public EntryBase {
  KEY _key;
  VAL _val;

public:
  Entry(KEY k, VAL v) : _key(k), _val(v) {}
  const KEY& key() const { return _key; }
  const VAL& val() const { return _val; }
};

}