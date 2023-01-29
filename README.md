# search_engine is a final project for a C++ developer beginner course.

<h1>Objective</h1>
<p>Create a CLI search server with following capabilities</p>
<ul>
  <li>search performed over text documents in the same location as executable file</li>
  <li>  Using JSON interface (<a href="https://github.com/nlohmann/json">nlohmann_json library</a>) for:</li>
  <ul>
    <li>  <b>program configuration</b> is stored in a local config.json formatted cofig file</li>
    <li>  <b> search requests</b> taken from another requests.json file </li>
    <li>  <b>search results</b> are placed answers.json file</li>
  </ul>
  <li>Separate class for <b>periodic indexing</b> of a database is performed based on configuration settings</li>
  <li><b>Unit testing</b> performed using Google tests module</li>
</ul>
<h1>General overview</h1>
<p>Upon startup InvertedIndex class instance starts looking for a 'database' directory and searches all .txt files in it. Creates a list of files, reads them in and performs indexing.</p>
<p>After that JSON module initializes and attempts to open up config file and reads configuration settings as well as updates list of files for the database. Search requests are read from a requests.json file </p>
<p>Then Search Server is started and performes search over the index, handing over results to JSON module and recorded to answers.json file. Additionally results are displayed in the terminal</p>
<p>
  After this initial search start background check of file database to be indexed in three detached threads
  <ul>
    <li><b>monitor</b> thread of a JSON module that checks if time since last indexting doesn't exceed a setting in the config file. Sets a bool flag when this time exceeds the setting and signals other modules that re-indexing is required</li>
    <li><b>updateIndex</b> thread of InvertedIndex class receives the bool flag and performs reindexing when the flag is set. It performs indexing and sets another bool flag to signal JSON module that indexing was complete</li>
    <li><b>updateJSON</b> thread of a JSON module receives the flag from InvertedIndex class and updates file list in the config.json file. It also updates timestamp of when indexing was performed</li>
  </ul> 
  </p>
  <p>A command line interface also runs in the background ready for user commands. User has a choice of commands:
      <ul>
          <li>index. manually command the program to re-index database</li>
          <li>search. manually command the program to perform search. At this command the program will read requests from requests.json file and display results in the teminal only</li>
          <li>exit. stop executing the program</li>
      </ul> 
  </p>
