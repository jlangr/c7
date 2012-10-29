#pragma once

#include "Scanner.h"

// TODO create a custom matcher for this instead!
template <typename T> static void ASSERT_CURRENT_STATE(Scanner* scanner)
{
    ASSERT_TRUE(dynamic_cast<T*>(scanner->CurrentState()) != NULL);
}
