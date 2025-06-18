#ifndef COMMON_H
#define COMMON_H

struct BBox {
    float xtl;  // x top-left
    float ytl;  // y top-left
    float width;
    float height;

    explicit BBox() : xtl(0.0), ytl(0.0), width(0.0), height(0.0) {};
    explicit BBox(float _xtl, float _ytl, float _w, float _h) : xtl(_xtl), ytl(_ytl), width(_w), height(_h) {};
    BBox(const BBox &_box) : xtl(_box.xtl), ytl(_box.ytl), width(_box.width), height(_box.height) {};
    BBox &operator=(const BBox &_box) {
        xtl = _box.xtl;
        ytl = _box.ytl;
        width = _box.width;
        height = _box.height;
        return *this;
    };
};

struct SotInfo {
    BBox bbox;
    float score;

    explicit SotInfo() : bbox(BBox()), score(0.0) {};
    explicit SotInfo(BBox _box, float _score) : bbox(_box), score(_score) {};
    SotInfo(const SotInfo &_info) : bbox(_info.bbox), score(_info.score) {};
    SotInfo &operator=(const SotInfo &_info) {
        bbox = _info.bbox;
        score = _info.score;
        return *this;
    };
};

#endif  // COMMON_H
