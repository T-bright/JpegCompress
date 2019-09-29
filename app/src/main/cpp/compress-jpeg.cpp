#include <jni.h>
#include <string>
#include <android/bitmap.h>
#include <android/log.h>
#include <jpeglib.h>
#include <setjmp.h>

#define LOG_TAG "------compress-jpeg----"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


int generateJPEG(uint8_t *data, int w, int h, int quality,
                 const char *path, jboolean optimize) {
    int nComponent = 3;
    // jpeg的结构体，保存的比如宽、高、位深、图片格式等信息
    struct jpeg_compress_struct jcs;

    jpeg_error_mgr error;
    jcs.err = jpeg_std_error(&error);

    jpeg_create_compress(&jcs);
    // 打开输出文件 wb:可写byte
    FILE *f = fopen(path, "wb");
    if (f == NULL) {
        return 0;
    }
    // 设置结构体的文件路径
    jpeg_stdio_dest(&jcs, f);

    //设置压缩参数
    jcs.image_width = w;
    jcs.image_height = h;

    // 设置哈夫曼编码
    jcs.arith_code = false;
    jcs.input_components = nComponent;
    if (nComponent == 1)
        jcs.in_color_space = JCS_GRAYSCALE;
    else
        jcs.in_color_space = JCS_RGB;

    jpeg_set_defaults(&jcs);
    jcs.optimize_coding = optimize;
    jpeg_set_quality(&jcs, quality, TRUE);
    // 开始压缩，写入全部像素
    jpeg_start_compress(&jcs, TRUE);

    JSAMPROW row_pointer[1];
    int row_stride;
    row_stride = jcs.image_width * nComponent;
    while (jcs.next_scanline < jcs.image_height) {
        row_pointer[0] = &data[jcs.next_scanline * row_stride];
        jpeg_write_scanlines(&jcs, row_pointer, 1);
    }
    jpeg_finish_compress(&jcs);
    jpeg_destroy_compress(&jcs);
    fclose(f);

    return 1;
}


extern "C"
JNIEXPORT jint JNICALL
Java_com_tbright_jpegcompress_BitmapCompressUtils_nativeCompress(JNIEnv *env, jclass clazz,
                                                                 jobject bitmap, jint quality,
                                                                 jstring path_) {
    const char *path = env->GetStringUTFChars(path_, 0);

    //从bitmap获取argb数据
    AndroidBitmapInfo info;
    int getInfo_result = AndroidBitmap_getInfo(env, bitmap, &info);
    if (getInfo_result < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", getInfo_result);
        return 0;
    }
    uint8_t *pixels;
    int lookPixels_result = AndroidBitmap_lockPixels(env, bitmap, (void **) &pixels);
    if (lookPixels_result < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", lookPixels_result);
        return 0;
    }
    int w = info.width;
    int h = info.height;
    LOGE("bitmap info：width = %d  height = %d", w,h);
    int color;
    uint8_t r, g, b;//byte
    uint8_t *data = (uint8_t *) malloc(w * h * 3);
    uint8_t *temp = data;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            color = *((int *) pixels);
            r = ((color & 0x00FF0000) >> 16);
            g = ((color & 0x0000FF00) >> 8);
            b = color & 0x000000FF;
            *data = b;
            *(data + 1) = g;
            *(data + 2) = r;
            data = data + 3;
            pixels += 4;
        }
    }
    AndroidBitmap_unlockPixels(env, bitmap);
    int resultCode = generateJPEG(temp, w, h, quality, path, true);

    free(temp);
    AndroidBitmap_unlockPixels(env, bitmap);
    env->ReleaseStringUTFChars(path_, path);
    return resultCode;
}


extern "C"
JNIEXPORT jint JNICALL
Java_com_tbright_jpegcompress_BitmapCompressUtils_nativeCompressWithSize(JNIEnv *env, jclass clazz,
                                                                         jobject bitmap,
                                                                         jint quality,
                                                                         jstring path_,
                                                                         jint width, jint height) {
    const char *path = env->GetStringUTFChars(path_, 0);

    //从bitmap获取argb数据
    AndroidBitmapInfo info;
    int getInfo_result = AndroidBitmap_getInfo(env, bitmap, &info);
    if (getInfo_result < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", getInfo_result);
        return 0;
    }
    uint8_t *pixels;
    int lookPixels_result = AndroidBitmap_lockPixels(env, bitmap, (void **) &pixels);
    if (lookPixels_result < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", lookPixels_result);
        return 0;
    }
    int w = info.width;
    int h = info.height;
    int color;
    uint8_t r, g, b;//byte
    uint8_t *data = (uint8_t *) malloc(w * h * 3);
    uint8_t *temp = data;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            color = *((int *) pixels);
            r = ((color & 0x00FF0000) >> 16);
            g = ((color & 0x0000FF00) >> 8);
            b = color & 0x000000FF;
            *data = b;
            *(data + 1) = g;
            *(data + 2) = r;
            data = data + 3;
            pixels += 4;
        }
    }
    AndroidBitmap_unlockPixels(env, bitmap);
    int resultCode = generateJPEG(temp, width, height, quality, path, true);

    free(temp);
    AndroidBitmap_unlockPixels(env, bitmap);
    env->ReleaseStringUTFChars(path_, path);
    return resultCode;
}