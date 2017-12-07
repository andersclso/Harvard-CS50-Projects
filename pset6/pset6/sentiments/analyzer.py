import nltk #natural language toolkit

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        
        #declare arrays to hold extracted positive and negative words from txt files
        self.positiveWords = []
        self.negativeWords = []
        
        #open positive-words.txt for reading
        with open(positives) as lines:
            for line in lines:
                #ignore comments in txt file
                if not line.startswith(';'):
                    #strip whitespace and just store the word in array
                    self.positiveWords.append(line.strip())
        
        #open negative-words.txt for reading
        with open(negatives) as lines: 
            for line in lines:
                #ignore comments in txt file
                if not line.startswith(';'):
                    #strip whitespace and just store the word in array
                    self.negativeWords.append(line.strip())
                
    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        
        #set result/score to 0 for neutral
        result = 0
        #splits tweet string into array of words/emoticons
        tokenizer = nltk.tokenize.TweetTokenizer()
        #tokens is the array of words/emoticons
        tokens = tokenizer.tokenize(text)

        for token in tokens:
            #if the word is positive, result + 1
            if token.lower() in self.positiveWords:
                result += 1
            #if the word is negative, result - 1
            if token.lower() in self.negativeWords:
                result -= 1
        
        return result
        
