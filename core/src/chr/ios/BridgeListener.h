@protocol BridgeListener

- (void) handleMessage:(int)what body:(NSString*)body;
- (void) handleEvent:(int)eventId;

@end
