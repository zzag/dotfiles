class MessagesController < ApplicationController
    def index
        @messages = Message.all
        respond_with @messages
    end

    def show
        @message = Message.find(params[:id])
        respond_with @message
    end
end
