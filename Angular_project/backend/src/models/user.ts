import mongoose from "mongoose";

const Schema = mongoose.Schema;

let User = new Schema({
    firstname: {
        type: String
    },
    lastname: {
        type: String
    },
    username: {
        type: String
    },
    password: {
        type: String
    },
    type: {
        type: String
    },
    adress: {
        type: String
    },
    phone: {
        type: String
    },
    mail: {
        type: String
    },
    licenceNumber: {
        type: Number
    },
    specialization: {
        type: String
    },
    branch: {
        type: String
    },
    image: {
        type: String
    },
    added: {
        type: Boolean
    }
})

export default mongoose.model('UserModel', User, 'users')