#include <cmath>

#define __STDC_CONSTANT_MACROS
extern "C" {
#include <libavutil/tx.h>
}

#include "spek-fft.h"

class FFTPlanImpl : public FFTPlan
{
public:
    FFTPlanImpl(int nbits);
    ~FFTPlanImpl() override;

    void execute() override;

private:
    AVTXContext *cx;
    av_tx_fn tx_fn;
};


std::unique_ptr<FFTPlan> FFT::create(int nbits) {
    return std::unique_ptr<FFTPlan>(new FFTPlanImpl(nbits));
}

FFTPlanImpl::FFTPlanImpl(int nbits) : FFTPlan(nbits), cx(nullptr), tx_fn(nullptr) {
    int len = 1 << nbits;  // Length of the transform
    int ret = av_tx_init(&cx, &tx_fn, AV_TX_FLOAT_RDFT, 0 /* forward transform */, len, nullptr, 0);
    if (ret < 0) {
        // Handle error (e.g., throw an exception or log the failure)
        cx = nullptr;
    }
}

FFTPlanImpl::~FFTPlanImpl() {
    if (cx) {
        av_tx_uninit(&cx);  // Proper cleanup for av_tx_init
        av_free(cx);
    }
}

void FFTPlanImpl::execute() {
    if (!tx_fn || !cx) {
        // Handle the case where initialization failed
        return;
    }

    int n = this->get_input_size();
    float n2 = n * n;

    // Input and output buffers
    float *input = this->get_input();
    size_t output_size = 2 * ((n / 2) + 1) * sizeof(float);
    float *output = static_cast<float *>(av_malloc(output_size));
    if (!output) {
        // Handle memory allocation failure
        return;
    }

    // Perform the transform
    tx_fn(cx, output, input, sizeof(float));

    // Calculate magnitudes and set the output
    this->set_output(0, 10.0f * log10f(output[0] * output[0] / n2));
    this->set_output(n / 2, 10.0f * log10f(output[1] * output[1] / n2));
    for (int i = 1; i < n / 2; i++) {
        float re = output[i * 2];
        float im = output[i * 2 + 1];
        this->set_output(i, 10.0f * log10f((re * re + im * im) / n2));
    }

    // Cleanup the output buffer
    av_free(output);
}

