# pragma once

# define CURRENT_FILE_NAME					\
(											\
	strrchr(__FILE__, '/')					\
		? strrchr(__FILE__, '/') + 1		\
		: strrchr(__FILE__, '\\')			\
			? strrchr(__FILE__, '\\') + 1	\
			: __FILE__						\
)