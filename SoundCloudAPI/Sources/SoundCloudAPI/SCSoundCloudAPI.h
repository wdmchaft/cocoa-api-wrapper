/*
 * Copyright 2009 Ullrich Schäfer, Gernot Poetsch for SoundCloud Ltd.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 *
 * For more information and documentation refer to
 * http://soundcloud.com/api
 * 
 */

#import <Foundation/Foundation.h>

@class NXOAuth2Client;
@class SCSoundCloudAPIConfiguration, SCSoundCloudConnection;
@protocol SCSoundCloudAPIAuthenticationDelegate, SCSoundCloudConnectionDelegate;


typedef enum {
	SCResponseFormatXML,
	SCResponseFormatJSON
} SCResponseFormat;


@interface SCSoundCloudAPI : NSObject {
	SCSoundCloudAPIConfiguration *configuration;
	NXOAuth2Client *oauthClient;
	
	id<SCSoundCloudAPIAuthenticationDelegate> authDelegate;
	SCResponseFormat responseFormat;
	BOOL isAuthenticated;
}

@property (assign) id<SCSoundCloudAPIAuthenticationDelegate> authDelegate;
@property SCResponseFormat responseFormat;
@property (assign) BOOL isAuthenticated;	// this might change dynamicaly

/*!
 * initialize the api object
 */
- (id)initWithAuthenticationDelegate:(id<SCSoundCloudAPIAuthenticationDelegate>)authDelegate
					apiConfiguration:(SCSoundCloudAPIConfiguration *)configuration;

/*!
 * invokes a request using the specified HTTP method on the specified resource
 * pass your connection delegate here
 * returns a connection object that can be used to cancel the request
 */
- (SCSoundCloudConnection *)performMethod:(NSString *)httpMethod
							   onResource:(NSString *)resource
						   withParameters:(NSDictionary *)parameters
								  context:(id)context
					   connectionDelegate:(NSObject<SCSoundCloudConnectionDelegate> *)connectionDelegate;


#pragma mark Authentication

/*!
 * checks if authorized, and if not lets you know in the authDelegate
 */
- (void)requestAuthentication;

/*!
 * resets token to nil, and removes it from the keychain
 */
- (void)resetAuthentication;

/*!
 * When you app recieves the callback via it's callback URL pass it on to this method
 * returns YES if the redirectURL was handled
 */
- (BOOL)handleOpenRedirectURL:(NSURL *)redirectURL;

/*!
 * Use this method to pass Username & Password on Credentials flow
 */
- (void)authorizeWithUsername:(NSString *)username password:(NSString *)password;


@end


@protocol SCSoundCloudAPIAuthenticationDelegate <NSObject>
- (void)soundCloudAPIDidAuthenticate:(SCSoundCloudAPI *)scAPI;
- (void)soundCloudAPIDidResetAuthentication:(SCSoundCloudAPI *)scAPI;
- (void)soundCloudAPI:(SCSoundCloudAPI *)scAPI didFailToGetAccessTokenWithError:(NSError *)error;

- (void)soundCloudAPI:(SCSoundCloudAPI *)scAPI preparedAuthorizationURL:(NSURL *)authorizationURL;
@end