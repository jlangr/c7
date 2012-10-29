#pragma once

#include "Book.h"

namespace ClassificationData {
    const std::string SEVEN_TITLE = "Se7en";
    const std::string SEVEN_AUTHOR = "Fincher";
    const unsigned short SEVEN_YEAR = 1995;
    const std::string SEVEN_CLASSIFICATION = "VABC 123";
    const Book SEVEN(
        SEVEN_TITLE, SEVEN_AUTHOR,
        SEVEN_YEAR, SEVEN_CLASSIFICATION, Book::TYPE_MOVIE);

	const std::string THE_TRIAL_TITLE = "Trial, The";
	const std::string THE_TRIAL_AUTHOR = "Kafka, Franz";
	const std::string THE_TRIAL_CLASSIFICATION = "PZ3.K11 Tr2";
	const unsigned short THE_TRIAL_YEAR = 1937;
	const Book THE_TRIAL(
		THE_TRIAL_TITLE, THE_TRIAL_AUTHOR,
		THE_TRIAL_YEAR, THE_TRIAL_CLASSIFICATION);

	const std::string CATCH22_TITLE = "Catch-22";
	const std::string CATCH22_AUTHOR = "Heller, Joseph";
	const std::string CATCH22_CLASSIFICATION = "PZ4.H47665";
	const unsigned short CATCH22_YEAR = 1961;
	const Book CATCH22(
		CATCH22_TITLE, CATCH22_AUTHOR,
		CATCH22_YEAR, CATCH22_CLASSIFICATION);

	const std::string CASH_AUTOBIO_TITLE = "Cash: The Autobiography";
	const std::string CASH_AUTOBIO_AUTHOR = "Cash, John R.";
	const std::string CASH_AUTOBIO_CLASSIFICATION = "ML420.C265 A3 1997";
	const unsigned short CASH_AUTOBIO_YEAR = 1997;
	const Book CASH_AUTOBIO(
		CASH_AUTOBIO_TITLE, CASH_AUTOBIO_AUTHOR,
		CASH_AUTOBIO_YEAR, CASH_AUTOBIO_CLASSIFICATION);
};
