#pragma once
#include <string>
#include <chrono>
#include <functional>
#include <vector>

class Animation {
public:
    // 文字动画
    static void Typewriter(const std::string& text, int delayMs = 50);
    static void TypewriterInBox(const std::string& text, int delayMs, int lineNumber);
    // 播放动画序列
    static void PlaySequence(const std::string& dirPath, int frameDelayMs);

private:
    // 获取排序后的动画帧文件
    static std::vector<std::string> GetSortedFrames(const std::string& dirPath);
    
    // 提取文件名中的数字序号
    static int ExtractFrameNumber(const std::string& filename);

    // 比较动画文件
    static bool CompareAnimationFiles(const std::string& a, const std::string& b);
};