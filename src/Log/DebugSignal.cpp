
#ifndef NDEBUG

#include <csignal>
#include <cstdlib>
#include <iostream>

#define FALSE 0
#define TRUE  1

std::sig_atomic_t segmentation_fault = FALSE;
std::sig_atomic_t erroneous_arithmetic_operation = FALSE;
std::sig_atomic_t invalid_program_image = FALSE;
std::sig_atomic_t abnormal_termination_condition = FALSE;

struct SIGNAL_ERROR {
        enum SIGNAL_ERROR_CODE {
                SEGMENTATION_FAULT,
                ERRONEOUS_ARITHMETIC_OPERATOR,
                INVALID_PROGRAM_IMAGE,
                ABNORMAL_TERMINATION_CONDITION,
        };
};


extern "C" {

[[noreturn]] static void LumenExit(int exit_code)
{
        std::exit(exit_code);
        //std::quick_exit(exit_code);
}

static void SegmentationFaultHandler([[maybe_unused]] int sig)
{
        segmentation_fault = TRUE;
        LumenExit(SIGNAL_ERROR::SEGMENTATION_FAULT);
}

static void ErroneousArithmeticOperationHandler([[maybe_unused]] int sig)
{
        erroneous_arithmetic_operation = TRUE;
        LumenExit(SIGNAL_ERROR::ERRONEOUS_ARITHMETIC_OPERATOR);
}

static void InvalidProgramImageHandler([[maybe_unused]] int sig)
{
        invalid_program_image = TRUE;
        LumenExit(SIGNAL_ERROR::INVALID_PROGRAM_IMAGE);
}

static void AbnormalTerminationConditionHandler([[maybe_unused]] int sig)
{
        abnormal_termination_condition = TRUE;
        LumenExit(SIGNAL_ERROR::ABNORMAL_TERMINATION_CONDITION);
}

static void LogSignal(void)
{

        if (segmentation_fault) {
                std::cerr << "[LogSignal] segmentation fault\n";
        }
        if (erroneous_arithmetic_operation) {
                std::cerr << "[LogSignal] erroneous arithmetic operation\n";
        }
        if (invalid_program_image) {
                std::cerr << "[LogSignal] invalid program image\n";
        }
        if (abnormal_termination_condition) {
                std::cerr << "[LogSignal] abnormal termination condition\n";
        }
}

static int SetupDebugSignal(void)
{
        std::cout << "[SetupDebugSignal]\n";
        //std::at_quick_exit(&LogSignal);
        std::atexit(&LogSignal);

        signal(SIGSEGV, &SegmentationFaultHandler);
        signal(SIGFPE, &ErroneousArithmeticOperationHandler);
        signal(SIGILL, &InvalidProgramImageHandler);
        signal(SIGABRT, &AbnormalTerminationConditionHandler);

        return 0;
}

[[maybe_unused]] static int Unused = SetupDebugSignal();
} // extern "C"

#endif // NDEBUG
