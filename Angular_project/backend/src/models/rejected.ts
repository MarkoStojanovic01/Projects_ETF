import mongoose from "mongoose";

const Schema = mongoose.Schema;

let Rejected = new Schema({
    username: {
        type: String
    },
    mail: {
        type: String
    }
})

export default mongoose.model('RejectedModel', Rejected, 'rejected')