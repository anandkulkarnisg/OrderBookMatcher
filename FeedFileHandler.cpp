#include "FeedFileHandler.h"

// Constructor : It takes in file from which each line is to be parsed and the ThreadPool which is passed via reference. 
FeedFileHandler::FeedFileHandler(const std::string& fileName, ThreadPool& pool) : m_feedFileName(fileName) , m_pool(pool) {  }

// buildTupleItems : This function builds the tuple items which is esentially parsing the file line by line and creating the vector of tuple items from which processing can be done.
// processing is done when process function is invoked via public interface.

void FeedFileHandler::buildTupleItems()
{
	// First aim to open the file and print the lines.
	m_inputFileStream.open(m_feedFileName, std::ifstream::in);

	std::string textLine;

	// Process the lines now.
	while(!m_inputFileStream.eof())
	{
		getline(m_inputFileStream, textLine);
		if(textLine.length()>0)
		{
			// We parse the line here and seperate the items.
			std::vector<std::string> tokenItems;
			char_separator<char> seperator("|");
			tokenizer<char_separator<char>> tokens(textLine, seperator);
			for(const auto& iter : tokens)
				tokenItems.push_back(iter);	
			m_tupleItems.push_back(std::make_tuple(tokenItems[0], tokenItems[1], tokenItems[2][0]));
		}
	}

	m_inputFileStream.close();
}

// process : This function calls the buildTupleItems currently.
void FeedFileHandler::process()
{
	// First build the internal tuple items that hold the data for all items to be processed.
	buildTupleItems();

	// Now iterate through each item and process it by submitting to pool.
	for(const auto& iter : m_tupleItems)
	{
		StockMatcher matcher(std::get<1>(iter), std::get<0>(iter), outputStreamType::fileStream);
		auto func = std::bind(&StockMatcher::executeMatching, &matcher);
		std::vector<std::future<void>> results;
		results.emplace_back(m_pool.enqueue(func));
		for(auto&& iter : results)
			iter.get();
	}
}
