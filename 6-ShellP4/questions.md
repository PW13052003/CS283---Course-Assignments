1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

The remote client determines the end of a command's output using the EOF character (RDSH_EOF_CHAR) sent by the server after executing a command. In order to handle partial reads or ensure complete message transmission, we can use fixed-length headers or recv() to return partial data. A loop will continue receiving until the EOF character is detected.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

Since TCP is a stream-based protocol and does not preserve message boundaries, a networked shell must explicitly define message start and end markers. One way to do this is by appending a predefined character (RDSH_EOF_CHAR) at the end of a command response to signal completion. If not handled correctly, TCP may split a single message across multuple packets causing partial reads. On the flip side, it is also possible for multiple messages to be received in a single TCP packet which will make it hard to separate commands. 

3. Describe the general differences between stateful and stateless protocols.

Stateful protocols maintain session state across multiple client-server interactions whereas stateless protocols treat each request as independent, with no memory of previous interactions. In a stateful protocol, the server tracks information between requests. However, in stateless protocols, the client must send all necessary information with each request. TCP is an example of a stateful protocol whereas UDP is an example of stateless protocol.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

UDP is a faster protocol for real time applications because it does not have a connection setup like TCP. It can also send data to multiple clients simultaneously which means it can be particularly helpful in live streaming streaming.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

The OS provides Sockets API to enable applications to use network communication.

